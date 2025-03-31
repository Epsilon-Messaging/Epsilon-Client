#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/asio.hpp>
#include <iostream>

//namespace beast = boost::beast;
//namespace http = beast::http;
//namespace net = boost::asio;
//using tcp = net::ip::tcp;
//namespace ssl = net::ssl;
//
//int main() {
//    try {
//        // I/O and SSL context
//        net::io_context ioc;
//        ssl::context ctx{ssl::context::sslv23_client};
//
//        // Resolve domain
//        tcp::resolver resolver{ioc};
//        auto const results = resolver.resolve("www.example.com", "https");
//
//        // Create SSL stream
//        beast::ssl_stream<beast::tcp_stream> stream{ioc, ctx};
//
//        // Connect to the resolved endpoint
//        beast::get_lowest_layer(stream).connect(results);
//
//        // Perform SSL handshake
//        stream.handshake(ssl::stream_base::client);
//
//        // Create an HTTP GET request
//        http::request<http::string_body> req{http::verb::get, "/", 11};
//        req.set(http::field::host, "www.example.com");
//        req.set(http::field::user_agent, "Boost.Beast/HelloWorld");
//
//        // Send the request
//        http::write(stream, req);
//
//        // Receive response
//        beast::flat_buffer buffer;
//        http::response<http::dynamic_body> res;
//        http::read(stream, buffer, res);
//
//        // Print response
//        std::cout << res << std::endl;
//
//        // Graceful shutdown
//        beast::error_code ec;
//        stream.shutdown(ec);
//        if (ec && ec != beast::errc::not_connected) {
//            throw beast::system_error{ec};
//        }
//    } catch (const std::exception &e) {
//        std::cerr << "Error: " << e.what() << std::endl;
//    }
//
//    return 0;
//}
//
#include <memory>
#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
 
using namespace ftxui;
 
int main() {
  auto screen = ScreenInteractive::Fullscreen();
 
  auto middle = Renderer([] { return text("middle") | center; });
  auto left = Renderer([] { return text("Left") | center; });
  auto right = Renderer([] { return text("right") | center; });
  auto top = Renderer([] { return text("top") | center; });
  auto bottom = Renderer([] { return text("bottom") | center; });
 
  int left_size = 40;
  int right_size = 40;
  int top_size = 1;
  int bottom_size = 5;
 
  auto container = middle;
  container = ResizableSplitBottom(bottom, container, &bottom_size);
  container = ResizableSplitLeft(left, container, &left_size);
  container = ResizableSplitRight(right, container, &right_size);
  container = ResizableSplitTop(top, container, &top_size);
 
  auto renderer =
      Renderer(container, [&] { return container->Render() | border; });
 
  screen.Loop(renderer);
}
