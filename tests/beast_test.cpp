
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE Main
#include <boost/test/unit_test.hpp>

//#include <boost/beast.hpp>
//#include <windows.h>
//namespace http = boost::beast::http;
//BOOST_AUTO_TEST_CASE(FristTest){
//  try{
//  boost::asio::io_context io_ctx;
//  boost::asio::ip::tcp::resolver dns_resolver(io_ctx);
//  auto const server_info = dns_resolver.resolve("www.baidu.com", "80");
//  boost::beast::tcp_stream data_stream(io_ctx);
//  http::request<http::string_body> req{http::verb::get, "/", 11};
//  req.set(http::field::host, "www.baidu.com");
//  req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
//  data_stream.connect(server_info);
//  http::write(data_stream, req);
//  http::response<http::dynamic_body> res;
//  boost::beast::flat_buffer buffer;
//  http::read(data_stream, buffer, res);
//
//  std::cout << res << std::endl;
//
//  boost::beast::error_code ec;
//  data_stream.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
//  BOOST_TEST(server_info.size());
//  } catch (std::exception& e){
//    BOOST_TEST_MESSAGE(e.what());
//  }
//}
//
//#include <boost/spirit/home/x3.hpp>
//#include "../source/html_parser/dom/attribute.h"
//
//
//namespace spirit = boost::spirit::x3;
//
//class attribute_data {
//  std::string name;
//  std::string value;
//};
//
//const auto test = spirit::lexeme[+spirit::char_];
//boost::spirit::x3::rule<class attribute_resolver, attribute_data> const attribute_resolve = "switch_action"; /* NOLINT */
//auto const attribute_resolve_def = spirit::lexeme[+spirit::char_];
//BOOST_SPIRIT_DEFINE(attribute_resolve)