#include "api.h"

#include <cpprest/http_client.h>

#include "external/json.hpp"
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

    static const auto API_BASE = utility::string_t(L"https://discordapp.com/api");

    json raw_request(web::http::method type, utility::string_t endpoint)
    {
      http_client client(API_BASE);
      http_request request(type);
      request.set_request_uri(endpoint);

      pplx::task<json> requestTask = client.request(request).then([=](http_response res) -> json
      {
        if (res.status_code())
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
        else
        {
          BOOST_LOG_TRIVIAL(error) << "Did not get proper response from API call (" << res.status_code() << ")";
          return json();
        }
      });

      requestTask.wait();
      return requestTask.get();
    }

    json request(RequestType type, utility::string_t endpoint)
    {
      BOOST_LOG_TRIVIAL(debug) << "Request: (" << detail::get_method_name(type) << ") - " << endpoint;
      return raw_request(detail::get_method(type), endpoint);
    }

    std::string get_wss_url()
    {
      auto response = request(RequestType::GET, L"gateway");
      return response["url"];
    }
  }
}
