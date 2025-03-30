#include <httplib.h>
#include <iostream>

int main() {
    httplib::Client cli("https://example.com");

    auto res = cli.Get("/");

    if (res) {
        std::cout << "Status: " << res->status << std::endl;

        if (res->status == 200) {
            std::cout << "Response body length: " << res->body.length() << " bytes" << std::endl;
            std::cout << "First 100 characters of response:" << std::endl;
            std::cout << res->body.substr(0, 100) << "..." << std::endl;
        }
    } else {
        std::cout << "Error: " << httplib::to_string(res.error()) << std::endl;
    }

    return 0;
}

