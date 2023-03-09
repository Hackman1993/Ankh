//
// Created by Hackman.Lo on 3/8/2023.
//

#ifndef ZK_CPP_SPIDER_SELECTOR_PARSER_H
#define ZK_CPP_SPIDER_SELECTOR_PARSER_H

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include "ast/html.h"
namespace ankh::html{
  class html_element;
  namespace css3::parser {

    std::unique_ptr<std::vector<html::css3::ast::css_single_element>> parse_ast(std::string source);
    std::unordered_map<html_element *, std::reference_wrapper<html_element>> execute_query_(const std::vector<html::css3::ast::css_single_element> &css_ast, const std::unordered_map<html_element *, std::reference_wrapper<html_element>> &input_elements);

    std::unordered_map<html_element*,std::reference_wrapper<html_element>> execute_query_m(std::string source, const std::unordered_map<html_element*, std::unique_ptr<html_element>>& elements_);

    std::vector<std::reference_wrapper<html_element>> execute_query_v(std::string source, const std::unordered_map<html_element*, std::unique_ptr<html_element>>& elements_);
  }
}

#endif //ZK_CPP_SPIDER_SELECTOR_PARSER_H
