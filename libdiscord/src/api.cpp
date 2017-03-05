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
        LOG(INFO) << "Setting request data: " << data.dump();
        request.set_body(data.dump());
      }

      pplx::task<json> requestTask = client.request(request).then([=](http_response res) -> json
      {
        if (res.status_code() == status_codes::OK)
        {
          auto bodyStream = res.body();
          container_buffer<std::string> inStringBuffer;
          return bodyStream.read_to_end(inStringBuffer).then([inStringBuffer](size_t bytesRead)
          {
            return inStringBuffer.collection();
          }).then([res](std::string text)
          {
            auto payload = json::parse(text.c_str());

            auto headers = res.headers();

            auto limit = headers.find(U("X-RateLimit-Limit"));
            auto remaining = headers.find(U("X-RateLimit-Remaining"));
            auto reset = headers.find(U("X-RateLimit-Reset"));

            if (limit != std::end(headers))
            {
              payload["X-RateLimit-Limit"] = utility::conversions::to_utf8string(limit->second);
            }

            if (remaining != std::end(headers))
            {
              payload["X-RateLimit-Remaining"] = utility::conversions::to_utf8string(remaining->second);
            }

            if (reset != std::end(headers))
            {
              payload["X-RateLimit-Reset"] = utility::conversions::to_utf8string(reset->second);
            }

            payload["response_status"] = res.status_code();

            return payload;
          }).get();
        }
        else if ((type == methods::DEL || type == methods::PUT) && res.status_code() == status_codes::NoContent)
        {
          //  NoContent is returned when a DELETE or PUT operation succeeds.
          return{ { "response_status", res.status_code() } };
        }
        else
        {
          auto json_str = utility::conversions::to_utf8string(res.extract_string().get());
          auto response = json::parse(json_str.c_str());

          auto code = response["code"].get<int>();
          auto message = response["message"].get<std::string>();

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
      });

      requestTask.wait();
      return requestTask.get();
    }

    nlohmann::json request(APICall& key, RequestType type, nlohmann::json data)
    {
      LOG(INFO) << "Request: (" 
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
        LOG(INFO) << "Could not lock global mutex. Waiting for it to unlock.";
        std::lock_guard<std::mutex> global_lock(GlobalMutex);
        LOG(INFO) << "Global mutex unlocked.";
      }

      auto response = raw_request(detail::get_method(type), utility::conversions::to_string_t(key.endpoint()), data);

      if (response["response_status"].get<int>() == 429)
      {
        LOG(INFO) << "Locking global mutex due to 429 response.";
        //  Lock the global mutex and wait until our rate limit is over.
        std::lock_guard<std::mutex> global_lock(GlobalMutex);
        auto reset_time = response["X-RateLimit-Reset"].get<uint32_t>();
        auto end_time = std::chrono::system_clock::time_point(std::chrono::seconds(reset_time));

        auto total_time = std::chrono::duration_cast<std::chrono::seconds>(end_time - std::chrono::system_clock::from_time_t(0)).count();
        LOG(WARNING) << "We hit the rate limit. Sleeping for " << total_time << " seconds.";
        std::this_thread::sleep_until(end_time);
      }

      LOG(INFO) << "Returning response.";

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
