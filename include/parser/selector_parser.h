//
// Created by Hackman.Lo on 3/8/2023.
//

#ifndef ZK_CPP_SPIDER_SELECTOR_PARSER_H
#define ZK_CPP_SPIDER_SELECTOR_PARSER_H

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

namespace ankh::ast{
  class css_single_element;
}
namespace ankh::html{
  class html_element;
  namespace css3::parser {

    std::unique_ptr<std::vector<ankh::ast::css_single_element>> parse_ast(std::string source);
    std::unordered_map<html_element *, std::reference_wrapper<html_element>> execute_query_(std::vector<ankh::ast::css_single_element> &css_ast, const std::unordered_map<html_element *, std::reference_wrapper<html_element>> &input_elements);

    std::unordered_map<html_element*,std::reference_wrapper<html_element>> execute_query_m(std::string source, const std::unordered_map<html_element*, std::unique_ptr<html_element>>& elements_);

    std::vector<std::reference_wrapper<html_element>> execute_query_v(std::string source, const std::unordered_map<html_element*, std::unique_ptr<html_element>>& elements_)
    {
      auto result = execute_query_m(std::move(source), elements_);

      std::vector<std::reference_wrapper<html_element>> res;
      for(auto& fin_res: result){
        res.emplace_back(fin_res.second);
      }
      return res;

    }
  }
}

#endif //ZK_CPP_SPIDER_SELECTOR_PARSER_H
