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

#include "components/MessageInput.hpp"
#include "components/CenterContent.hpp"
#include "components/SidebarLeft.hpp"
#include "components/SidebarRight.hpp"
#include "components/TitleBar.hpp"
 
using namespace ftxui;
 
int main() {
   auto screen = ScreenInteractive::Fullscreen();

    std::string input_content;
    
    int input_height = 5;
    auto header_panel = Epsilon::CreateHeaderPanel();
    auto left_panel = Epsilon::CreateLeftPanel();
    auto right_panel = Epsilon::CreateRightPanel();
    auto main_panel = Epsilon::CreateMainPanel();
    auto input_panel = Epsilon::CreateInputPanel(&input_content);
    
    int left_size = 40;
    int right_size = 40;
    int top_size = 1;
    
    auto main_with_input = Container::Vertical({
        main_panel | flex,  // Main panel takes all available space
        input_panel | size(HEIGHT, EQUAL, input_height)  // Input panel has fixed height
    });
    
    auto container = main_with_input;
    container = ResizableSplitLeft(left_panel, container, &left_size);
    container = ResizableSplitRight(right_panel, container, &right_size);
    container = ResizableSplitTop(header_panel, container, &top_size);

    auto renderer = Renderer(container, [&] { return container->Render() | border; });

    screen.Loop(renderer);
}
