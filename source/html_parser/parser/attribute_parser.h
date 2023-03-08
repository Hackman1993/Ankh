//
// Created by Hackman.Lo on 11/20/2022.
//

#ifndef ZK_CPP_SPIDER_ATTRIBUTE_PARSER_H
#define ZK_CPP_SPIDER_ATTRIBUTE_PARSER_H

#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>
#include "../ast/attribute.h"
#include "common.h"



namespace ankh::parser::html{
  using namespace boost::spirit::x3;
  struct attribute_parser_id;


  struct attribute_parser_id: ankh::parser::common::error_handler, annotate_on_success{};

  auto attribute_identifier_= rule<attribute_parser_id, std::string>{"AttributeIdentifier"} = lexeme[+(~char_("< /=>"))];
  auto attribute_value_= rule<attribute_parser_id, std::string>{"AttributeValue"} =
      lexeme["\"" > *(~char_("\"")) > "\""]|lexeme["'" > *(~char_("'")) > "'"]|
      lexeme[+(~char_(" />"))];
  auto single_attribute_ = rule<attribute_parser_id, ankh::ast::attribute_data>{"SingleAttribute"} = attribute_identifier_ > -("=" > attribute_value_);
  auto attributes_ = rule<attribute_parser_id, std::vector<ankh::ast::attribute_data>>{"Attributes"} = (*single_attribute_);
}


#endif //ZK_CPP_SPIDER_ATTRIBUTE_PARSER_H
