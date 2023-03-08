#include <boost/spirit/home/x3.hpp>
#include "../source/html_parser/constructure/html_document.h"
#include <boost/optional.hpp>
#include <fstream>
#include <boost/test/unit_test.hpp>
#define BOOST_SPIRIT_X3_DEBUG

namespace spirit = boost::spirit::x3;
BOOST_AUTO_TEST_SUITE(Spirit)

  BOOST_AUTO_TEST_CASE(CSS3SelectorParserTest){
    std::string source = "<!DOCTYPE html>\n"
 "<html>\n"
 "    <head>\n"
 "        <style type=\"text/css\" disabled>\n"
 "            p{\n"
 "        background-color: red;\n"
 "    }\n"
 "        </style>\n"
 "    </head>\n"
 "    <body>\n"
 "        <input type=\"button\" checked/>\n"
 "        <audio checked></audio>\n"
 "        <select checked>\n"
 "            <option value=\"1\">data1</option>\n"
 "            <option value=\"2\" checked>data2</option>\n"
 "            <option value=\"3\" checked>data3</option>\n"
 "            <option value=\"4\">data4</option>\n"
 "            <option value=\"5\">data5</option>\n"
 "        </select>\n"
 "    </body>\n"
 "</html>";
    ankh::html::html_document document;
    bool result = document.parse(source);
    auto ret = document.querySelector(":checked");
    for(auto &data: ret){
      std::cout << data.get().to_string() <<std::endl;
    }
    BOOST_CHECK(result);
  }



  BOOST_AUTO_TEST_CASE(HTMLParseTest) {
    std::string source = "<div id='bcd'>BCD<script>console.log('bcd')</script>AA<div/></div>";
    ankh::html::html_document document;
    bool result = document.parse(source);
    BOOST_CHECK(result);
  }

  BOOST_AUTO_TEST_CASE(multiple_attributes) {
    std::string source = "data-src=\"https://www.baidu.com/index.php\" id='hello world' visible bbc=ccd ccd=12.151 aac=true";
    std::vector<ankh::ast::attribute_data> result;
    using error_handler_type = boost::spirit::x3::error_handler<std::string::const_iterator>;
    error_handler_type error_handler(source.begin(), source.end(), std::cerr);

    auto const parser = spirit::with<spirit::error_handler_tag>(std::ref(error_handler))[ankh::parser::html::attributes_];
    auto parse_result = spirit::phrase_parse(source.begin(), source.end(), parser, spirit::ascii::space, result);

    BOOST_CHECK(parse_result);
    BOOST_CHECK_EQUAL(result.size(), 6);
    if (result.size() == 6) {
      BOOST_CHECK_EQUAL(result[0].name, "data-src");
      BOOST_CHECK_EQUAL(result[0].value.get_value_or("true"), "https://www.baidu.com/index.php");
      BOOST_CHECK_EQUAL(result[1].name, "id");
      BOOST_CHECK_EQUAL(result[1].value.get_value_or("true"), "hello world");
      BOOST_CHECK_EQUAL(result[2].name, "visible");
      BOOST_CHECK_EQUAL(result[2].value.get_value_or("true"), "true");
      BOOST_CHECK_EQUAL(result[3].name, "bbc");
      BOOST_CHECK_EQUAL(result[3].value.get_value_or("true"), "ccd");
      BOOST_CHECK_EQUAL(result[4].name, "ccd");
      BOOST_CHECK_EQUAL(result[4].value.get_value_or("true"), "12.151");
      BOOST_CHECK_EQUAL(result[5].name, "aac");
      BOOST_CHECK_EQUAL(result[5].value.get_value_or("true"), "true");
    }
  }

BOOST_AUTO_TEST_SUITE_END()