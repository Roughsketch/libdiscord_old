#include "api.h"
#include "common.h"

#include <future>

namespace ModDiscord
{
  namespace API
  {
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
        case DELETE:
          return web::http::methods::DEL;
        case PATCH:
          return web::http::methods::PATCH;
        }

        BOOST_LOG_TRIVIAL(error) 
          << "Invalid RequestType sent to ModDiscord::API::detail::get_method (" 
          << static_cast<int>(type) 
          << ")";

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
        case DELETE:
          return "DEL";
        case PATCH:
          return "PATCH";
        }

        BOOST_LOG_TRIVIAL(error)
          << "Invalid RequestType sent to ModDiscord::API::detail::get_method_name ("
          << static_cast<int>(type)
          << ")";

        return{};
      }
    }

    using namespace web::http;
    using namespace web::http::client;
    using namespace Concurrency::streams;

    using namespace nlohmann;

    static const auto API_BASE = utility::string_t(L"https://discordapp.com/api/v6");

    json raw_request(web::http::method type, utility::string_t endpoint, nm::json data)
    {
      http_client client(API_BASE);
      http_request request(type);
      request.set_request_uri(endpoint);
      request.headers().add(L"Authorization", Token);
      request.headers().add(L"Content-Type", L"application/json");
      
      if (!data.empty())
      {
        BOOST_LOG_TRIVIAL(info) << "Setting request data: " << data.dump();
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
          }).then([](std::string text)
          {
            return json::parse(text.c_str());
          }).get();
        }
        else if ((type == methods::DEL && res.status_code() == status_codes::NoContent))
        {
          return{ { "response_status", res.status_code() } };
        }
        else
        {
          BOOST_LOG_TRIVIAL(error) << "Did not get proper response from API call (" << res.status_code() << ") - " << res.extract_string().get().c_str();
          return json();
        }
      });

      requestTask.wait();
      return requestTask.get();
    }

    json request(RequestType type, std::string endpoint, nm::json data)
    {
      BOOST_LOG_TRIVIAL(debug) << "Request: (" << detail::get_method_name(type) << ") - " << endpoint;
      return raw_request(detail::get_method(type), utility::conversions::to_string_t(endpoint), data);
    }

    void set_token(std::string token)
    {
      Token = utility::conversions::to_string_t(token);
    }

    std::string get_wss_url()
    {
      auto response = request(RequestType::GET, "gateway");
      return response["url"];
    }
  }
}
