//
// Created by Hackman.Lo on 3/9/2023.
//

#ifndef ZK_CPP_SPIDER_HTML_VISITOR_H
#define ZK_CPP_SPIDER_HTML_VISITOR_H
#include <memory>
#include "ast/html.h"
namespace ankh::html{
  class html_element;
  namespace parser {
    struct html_visitor {
      using result_type = std::unique_ptr<html_element>;
      result_type operator()(const ast::tag_header &data);
      result_type operator()(const ast::tag_footer &data) const;
      result_type operator()(const std::string &data) const ;
      result_type operator()(const ast::comment_tag &data) const ;
      result_type operator()(const ast::doctype_tag &data) const;
      result_type operator()(const ast::script_tag &data) const;
    };
  }
}
#endif //ZK_CPP_SPIDER_HTML_VISITOR_H
