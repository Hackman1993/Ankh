//
// Created by Hackman.Lo on 11/28/2022.
//

#ifndef ZK_CPP_SPIDER_HTML_PARSER_H
#define ZK_CPP_SPIDER_HTML_PARSER_H

#include "common.h"
#include "../ast/html.h"
namespace ankh::html{

  class html_element;
  namespace parser {
    std::vector<ast::html_element> parse_source(std::string &source);
    bool parse_ast(const std::vector<ast::html_element> &html_ast, std::unordered_map<html_element*, std::unique_ptr<html_element>>& elements_, std::vector<std::reference_wrapper<html_element>>& structure_);
  }
}


#endif //ZK_CPP_SPIDER_HTML_PARSER_H
