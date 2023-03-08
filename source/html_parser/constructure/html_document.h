//
// Created by Hackman.Lo on 2/23/2023.
//

#ifndef ZK_CPP_SPIDER_HTML_DOCUMENT_H
#define ZK_CPP_SPIDER_HTML_DOCUMENT_H
#include "html_element.h"
#include "../parser/tag_parser.h"
#include "../parser/selector_parser.h"
#include "css3_selectors.h"
#include <boost/spirit/home/x3.hpp>
namespace ankh::html{
  class html_document {
  public:
    bool parse(std::string& source){
      std::vector<ankh::ast::html_element> result;
      using error_handler_type = boost::spirit::x3::error_handler<std::string::const_iterator>;
      error_handler_type error_handler(source.begin(), source.end(), std::cerr);
      auto const parser = boost::spirit::x3::with<boost::spirit::x3::error_handler_tag>(std::ref(error_handler))[ankh::parser::html::html_element_];
      auto parse_result = phrase_parse(source.begin(), source.end(), *parser, boost::spirit::x3::ascii::space, result);
      if(parse_result)
        return parse_ast(result);
      return false;
    }

    std::vector<std::reference_wrapper<html_element>> querySelector(std::string source)
    {
      using error_handler_type = boost::spirit::x3::error_handler<std::string::const_iterator>;
      error_handler_type error_handler(source.begin(), source.end(), std::cerr);
      std::vector<ankh::ast::css_single_element> css_ast;
      //ankh::ast::css_single_element result;
      auto const parser = boost::spirit::x3::no_skip[boost::spirit::x3::with<boost::spirit::x3::error_handler_tag>(std::ref(error_handler))[ankh::parser::css3::css3_selector_parser_]];
      auto parse_result = boost::spirit::x3::phrase_parse(source.begin(), source.end(), parser, boost::spirit::x3::ascii::space, css_ast);

      std::unordered_map<html_element*,std::reference_wrapper<html_element>> final_result;
      std::unordered_map<html_element*,std::reference_wrapper<html_element>> input_elements;
      for(auto &item : elements_){
        input_elements.emplace(item.first, *item.second);
      }

      bool use_last_result = false;
      std::unordered_map<html_element*,std::reference_wrapper<html_element>> last_result;
      for(auto & selector: css_ast){
        std::unordered_map<html_element*,std::reference_wrapper<html_element>> next_input = use_last_result? last_result: input_elements;
        ankh::css3::Visitor visitor(next_input);
        auto helper = selector.apply_visitor(visitor);
        if(helper.final){
          for(auto &res: helper.result_){
            final_result.emplace(res);
          }
          helper.result_.clear();
          use_last_result = false;
        }else{
          use_last_result = true;
        }
        last_result = std::move(helper.result_);
      }

      for(auto &fin_data: last_result){
        final_result.emplace(fin_data);
      }
      std::vector<std::reference_wrapper<html_element>> res;
      for(auto& fin_res: final_result){
        res.emplace_back(fin_res.second);
      }
      return res;

    }
  protected:
    std::unordered_map<html_element*, std::unique_ptr<html_element>> elements_;
    std::vector<std::reference_wrapper<html_element>> structure_;

    bool parse_ast(const std::vector<ankh::ast::html_element>& element){
      std::stack<html_element*> element_stack;
      for(auto ast: element){
        html_element* current_parent = element_stack.empty() ? nullptr : element_stack.top();
        Visitor visitor;
        std::unique_ptr<html_element> result = ast.apply_visitor(visitor);
        switch (result->type()) {
          case html_element::ET_FOOTER:
            // 遇到的是关闭标签
            if(!current_parent) continue;
            if(current_parent->content() == result->content()){
              current_parent->set_closed(true);
              element_stack.pop();
            }else{
              throw std::logic_error(std::string("Unexpected close tag of ") + result->content());
            }
            break;
          case html_element::ET_NORMAL:
            if(!result->closed()) {
              // If this tag is not closed , push this tag to the stack
              element_stack.push(result.get());
            }
          case html_element::ET_INNER_TEXT:
          case html_element::ET_COMMENT:
          case html_element::ET_SCRIPT:
            result->set_parent(current_parent);
            if(current_parent){
              current_parent->append_child(*result);
            }else{
              structure_.emplace_back(*result);
            }
            elements_.emplace(result.get(),std::move(result));
        }
      }
      return true;
    }
  protected:
    friend class Visitor;
  };
}

#endif //ZK_CPP_SPIDER_HTML_DOCUMENT_H
