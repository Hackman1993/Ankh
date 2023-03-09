
#include <fstream>
#include <ankh.h>
#include <boost/test/unit_test.hpp>

namespace spirit = boost::spirit::x3;
BOOST_AUTO_TEST_SUITE(Spirit)

  BOOST_AUTO_TEST_CASE(CSS3SelectorParserTest){
    std::string source = "<!DOCTYPE parser>\n"
                         "<html>\n"
                         "    <body>\n"
                         "    <div id=\"test\"> </div>"
                         "    </body>\n"
                         "</html>";
    ankh::html::html_document document;
    bool result = document.parse(source);
    auto ret = document.select("#test");
    for(auto &data: ret){
      std::cout << data.get().to_string() <<std::endl;
    }
    BOOST_CHECK(result);
  }
BOOST_AUTO_TEST_SUITE_END()