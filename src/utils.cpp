#include "utils.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
namespace beast = boost::beast;
namespace http = beast::http;

std::string read_file(const std::string& path) {
  std::ifstream file(path);
  if (!file.is_open()) {
    throw std::runtime_error("Could not open file: " + path);
  }
  std::ostringstream contents;
  contents << file.rdbuf();
  return contents.str();
}

http::response<http::string_body> handle_request(
    const http::request<http::string_body>& req) {
  http::response<http::string_body> res;

  res.version(req.version());
  res.keep_alive(false);

  res.set(http::field::server, "http-beastie");
  res.set(http::field::content_type, "text/html");

  if (req.target() == "/") {
    res.result(http::status::ok);
    res.body() = read_file("../static/index.html");
  } else if (req.target() == "/essam") {
    res.result(http::status::ok);
    res.body() = read_file("../static/essam.html");
  } else {
    res.result(http::status::not_found);
    res.body() = "<h1 style=\"text-align: center;\">404 Not Found</h1>";
  }
  res.prepare_payload();
  return res;
}
