//
// Created by Hackman.Lo on 2/23/2023.
//

#ifndef ZK_CPP_SPIDER_HTML_DOCUMENT_H
#define ZK_CPP_SPIDER_HTML_DOCUMENT_H
#include "html_element.h"
#include "../parser/html_parser.h"
#include "parser/selector_visitor.h"
#include <boost/spirit/home/x3.hpp>
#include <utility>

#include "parser/selector_parser.h"
namespace ankh::html{
  class html_document {
  public:
    std::vector<std::reference_wrapper<html_element>> select(std::string source)
    {
      return css3::parser::execute_query_v(std::move(source), elements_);
    }

    bool parse(std::string source){
      auto ast = parser::parse_source(source);
      if(ast.empty())
        return false;

      parser::parse_ast(ast,elements_,  structure_);
    }
  protected:
    std::unordered_map<html_element*, std::unique_ptr<html_element>> elements_;
    std::vector<std::reference_wrapper<html_element>> structure_;
  };
}

#endif //ZK_CPP_SPIDER_HTML_DOCUMENT_H
