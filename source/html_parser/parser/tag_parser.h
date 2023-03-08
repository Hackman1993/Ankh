//
// Created by Hackman.Lo on 11/28/2022.
//

#ifndef ZK_CPP_SPIDER_TAG_PARSER_H
#define ZK_CPP_SPIDER_TAG_PARSER_H

#include "../ast/tag.h"
#include "attribute_parser.h"
#include "common.h"



namespace ankh::parser::html {
  using namespace boost::spirit::x3;

  struct tag_parser_id: ankh::parser::common::error_handler{};

  auto inner_text = lexeme[+(~char_("<"))];


  auto tag_name_ = rule<tag_parser_id, std::string>{"HtmlTagName"} = lexeme[*(~char_("< />"))];
  auto comment_ = rule<tag_parser_id,ankh::ast::comment_tag>{"CommentTag"} = "<!--" >> lexeme[*(char_ - "-->")] >> "-->";
  auto doctype_tag_ = rule<tag_parser_id,ankh::ast::doctype_tag>{"DoctypeTag"} = no_case["<!doctype"] >> lexeme[*~char_(">")] >> ">";
  auto script_tag_ = rule<tag_parser_id, ankh::ast::script_tag>{"ScriptTag"} = no_case["<script"] >> attributes_ >> ">" >> *(char_ - no_case["</script>"]) >> no_case["</script>"];
  auto tag_header_ = rule<tag_parser_id, ankh::ast::tag_header>{"HtmlTagHeader"} = '<' > tag_name_ > attributes_ > -('/' >> attr(true)) > '>';
  auto tag_footer_ = rule<tag_parser_id, ankh::ast::tag_footer>{"HtmlTagFooter"} = "</" > tag_name_ > '>';

  rule<tag_parser_id, ankh::ast::html_element> html_element_ = "HtmlElement";
  auto html_element__def = inner_text| comment_ | doctype_tag_ | script_tag_  | tag_footer_ | tag_header_  ;

  BOOST_SPIRIT_DEFINE(html_element_);
}

#endif //ZK_CPP_SPIDER_TAG_PARSER_H
