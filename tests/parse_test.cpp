#include <boost/spirit/home/x3.hpp>
#include "constructure/html_document.h"
#include <boost/optional.hpp>
#include <fstream>
#include <boost/test/unit_test.hpp>

namespace spirit = boost::spirit::x3;
BOOST_AUTO_TEST_SUITE(Spirit)

  BOOST_AUTO_TEST_CASE(CSS3SelectorParserTest){
    std::string source = "<!DOCTYPE parser>\n"
 "<parser>\n"
 "    <head>\n"
 "        <style type=\"text/css\" disabled>\n"
 "            p{\n"
 "        background-color: red;\n"
 "    }\n"
 "        </style>\n"
 "    </head>\n"
 "    <body>\n"
 "        <input type=\"checkbox\" checked/>\n"
 "        <audio checked></audio>\n"
 "        <select checked>\n"
 "            <option value=\"1\">data1</option>\n"
 "            <option value=\"2\" checked>data2</option>\n"
 "            <option value=\"3\" checked>data3</option>\n"
 "            <option value=\"4\">data4</option>\n"
 "            <option value=\"5\">data5</option>\n"
 "        </select>\n"
 "    </body>\n"
 "</parser>";
    ankh::html::html_document document;
    bool result = document.parse(source);
    auto ret = document.select(":checked");
    for(auto &data: ret){
      std::cout << data.get().to_string() <<std::endl;
    }
    BOOST_CHECK(result);
  }



  BOOST_AUTO_TEST_CASE(HTMLParseTest) {
//    std::string source = "<div id='bcd'>BCD<script>console.log('bcd')</script>AA<div/></div>";
//    ankh::html::html_document document;
//    bool result = document.parse(source);
//    BOOST_CHECK(result);
  }

  BOOST_AUTO_TEST_CASE(multiple_attributes) {

  }

BOOST_AUTO_TEST_SUITE_END()