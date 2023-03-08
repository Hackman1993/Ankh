//
// Created by Hackman.Lo on 11/27/2022.
//

#ifndef ZK_CPP_SPIDER_ATTRIBUTE_H
#define ZK_CPP_SPIDER_ATTRIBUTE_H

#include <string>
#include <boost/optional.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/home/x3/support/utility/error_reporting.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <iostream>

namespace ankh::ast {
  struct attribute_data : boost::spirit::x3::position_tagged {
    std::string name;
    boost::optional<std::string> value;
  };

  using boost::fusion::operator<<;
}
BOOST_FUSION_ADAPT_STRUCT(ankh::ast::attribute_data, name, value);

#endif //ZK_CPP_SPIDER_ATTRIBUTE_H
