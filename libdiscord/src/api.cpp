#include "api.h"
#include "common.h"

#include <future>
#include <cpprest/http_client.h>

namespace Discord
{
  namespace API
  {
    static utility::string_t Token;
    static std::mutex GlobalMutex;
    static std::map<size_t, std::unique_ptr<std::mutex>> APIMutex;

    namespace detail
    {
      web::http::method get_method(RequestType type)
      {
        switch(type)
        {
        case GET:
          return web::http::methods::GET;
        case POST:
          return web::http::methods::POST;
        case PUT:
          return web::http::methods::PUT;
        case DEL:
          return web::http::methods::DEL;
        case PATCH:
          return web::http::methods::PATCH;
        }

        LOG(ERROR) << "Invalid RequestType sent to Discord::API::detail::get_method (" << static_cast<int>(type) << ")";

        return {};
      }

      std::string get_method_name(RequestType type)
      {
        switch (type)
        {
        case GET:
          return "GET";
        case POST:
          return "POST";
        case PUT:
          return "PUT";
        case DEL:
          return "DEL";
        case PATCH:
          return "PATCH";
        }

        LOG(ERROR) << "Invalid RequestType sent to Discord::API::detail::get_method_name (" << static_cast<int>(type) << ")";
        return{};
      }
    }

    using namespace web::http;
    using namespace web::http::client;
    using namespace Concurrency::streams;

    using namespace nlohmann;

    static const auto API_BASE = U("https://discordapp.com/api/v6");

    json raw_request(web::http::method type, utility::string_t endpoint, nlohmann::json data)
    {
      http_client client(API_BASE);
      http_request request(type);
      request.set_request_uri(endpoint);
      request.headers().add(U("Authorization"), Token);
      request.headers().add(U("Content-Type"), U("application/json"));
      
      if (!data.empty())
      {
        if (type == web::http::methods::GET)
        {
          LOG(DEBUG) << "Setting query parameters: " << data.dump();
          uri_builder builder(endpoint);
          
          for (auto it = std::begin(data); it != std::end(data); ++it)
          {
            std::string value;

            if (it.value().is_string())
            {
              value = it.value().get<std::string>();
            }
            else if (it.value().is_number_integer())
            {
              value = std::to_string(it.value().get<int>());
            }
            
            builder.append_query( utility::conversions::to_string_t(it.key()),
                                  utility::conversions::to_string_t(value));
          }

          request.set_request_uri(builder.to_string());
        }
        else
        {
          LOG(DEBUG) << "Setting request data: " << data.dump();
          request.set_body(data.dump());
        }
      }

      pplx::task<json> requestTask = client.request(request).then([=](http_response res) -> json
      {
        //  A container to hold all our response stuff.
        nlohmann::json container = { { "response_status", res.status_code() } };

        auto headers = res.headers();

        //  Various rate-limit related headers
        auto limit = headers.find(U("X-RateLimit-Limit"));
        auto remaining = headers.find(U("X-RateLimit-Remaining"));
        auto reset = headers.find(U("X-RateLimit-Reset"));
        auto retry = headers.find(U("Retry-After"));
        auto global = headers.find(U("X-RateLimit-Global"));

        //  Add each type of rate limit header to our container if it exists.
        if (limit != std::end(headers))
        {
          container["X-RateLimit-Limit"] = std::stoull(utility::conversions::to_utf8string(limit->second));
        }

        if (remaining != std::end(headers))
        {
          container["X-RateLimit-Remaining"] = std::stoull(utility::conversions::to_utf8string(remaining->second));
        }

        if (reset != std::end(headers))
        {
          container["X-RateLimit-Reset"] = std::stoull(utility::conversions::to_utf8string(reset->second));
        }

        if (retry != std::end(headers))
        {
          container["Retry-After"] = std::stoull(utility::conversions::to_utf8string(retry->second));
        }

        if (global != std::end(headers))
        {
          container["X-RateLimit-Global"] = std::stoull(utility::conversions::to_utf8string(global->second));
        }

        container["response_status"] = res.status_code();


        if (res.status_code() == status_codes::OK)
        {
          auto bodyStream = res.body();
          container_buffer<std::string> inStringBuffer;
          bodyStream.read_to_end(inStringBuffer).then([inStringBuffer](size_t bytesRead)
          {
            return inStringBuffer.collection();
          }).then([&container](std::string text)
          {
            auto payload = json::parse(text.c_str());
            container["response_data"] = payload;
          }).get();
        }
        else if (res.status_code() != status_codes::NoContent)
        {
          auto json_str = utility::conversions::to_utf8string(res.extract_string().get());
          auto response = json::parse(json_str.c_str());

          container["response_data"] = response;
        }

        return container;
      });

      requestTask.wait();
      return requestTask.get();
    }

    nlohmann::json request(APICall& key, RequestType type, nlohmann::json data)
    {
      LOG(DEBUG) << "Request: ("
                << detail::get_method_name(type) 
                << ") - " << key.endpoint() 
                << " " << data.dump(2);

      auto map_key = key.hash();
      auto mutex_it = APIMutex.find(map_key);

      //  If the cached mutex does not exist, create it.
      if (mutex_it == std::end(APIMutex))
      {
        APIMutex[map_key] = std::make_unique<std::mutex>();
      }

      auto mutex = APIMutex[map_key].get();

      std::lock_guard<std::mutex> api_lock(*mutex);

      if (GlobalMutex.try_lock())
      {
        GlobalMutex.unlock();
      }
      else
      {
        //  If we can't lock the global mutex, then we might be rate limited.
        //  Wait for the global mutex to become available again before continuing.
        LOG(DEBUG) << "Could not lock global mutex. Waiting for it to unlock.";
        std::lock_guard<std::mutex> global_lock(GlobalMutex);
        LOG(DEBUG) << "Global mutex unlocked.";
      }

      //  Get result from the request.
      auto response = raw_request(detail::get_method(type), utility::conversions::to_string_t(key.endpoint()), data);
      auto rdata = response["response_data"];

      if (response.count("X-RateLimit-Global"))
      {
        auto wait_time = response["Retry-After"].get<uint32_t>();
        LOG(ERROR) << "Hit the global rate limit. Waiting for " << wait_time << "ms.";

        //  This is a global rate limit, lock the global mutex and wait.
        std::lock_guard<std::mutex> global_lock(GlobalMutex);
        std::this_thread::sleep_for(std::chrono::milliseconds(wait_time));
      }
      else if (response.count("X-RateLimit-Remaining") && !response["X-RateLimit-Remaining"].get<int>())
      {
        //  Get the time that the rate limit will reset.
        auto reset_time = response["X-RateLimit-Reset"].get<uint32_t>();
        auto end_time = std::chrono::system_clock::time_point(std::chrono::seconds(reset_time));

        //  Get the total amount of time to wait from this point.
        auto total_time = std::chrono::duration_cast<std::chrono::seconds>(end_time - std::chrono::system_clock::now()).count();

        LOG(WARNING) << "We hit the rate limit for endpoint " << key.endpoint() << ". Sleeping for " << total_time << " seconds.";
        std::this_thread::sleep_until(end_time);
      }
      else if (rdata.count("code"))
      {
        //  We got a response code from the request.
        if (rdata.count("name"))
        {
          auto name = rdata["name"].get<std::vector<std::string>>();

          if (name.size() > 0)
          {
            throw DiscordException(name[0]);
          }

          throw DiscordException("API call failed and response was null.");
        }

        auto code = rdata["code"].get<int>();
        auto message = rdata["message"].get<std::string>();

        if (code < 20000)
        {
          throw UnknownException(message);
        }

        if (code < 30000)
        {
          throw TooManyException(message);
        }

        switch (code)
        {
        case EmbedDisabled:
          throw EmbedException(message);
        case MissingPermissions:
        case ChannelVerificationTooHigh:
          throw PermissionException(message);
        case Unauthorized:
        case MissingAccess:
        case InvalidAuthToken:
          throw AuthorizationException(message);
        default:
          throw DiscordException(message);
        }
      }

      if (response.count("response_data"))
      {
        return response["response_data"];
      }

      return response;
    }

    void set_token(std::string token)
    {
      Token = utility::conversions::to_string_t(token);
    }

    std::string get_wss_url()
    {
      auto response = request(APICall() << "gateway", GET);
      return response["url"];
    }
  }
}
