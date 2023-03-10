//
// Created by Hackman.Lo on 2/14/2023.
//

#ifndef ZK_CPP_SPIDER_HTML_ELEMENT_H
#define ZK_CPP_SPIDER_HTML_ELEMENT_H
#include <string>
#include <utility>
#include <vector>
#include <memory>
#include <stack>
#include "../ast/html.h"
#include <unordered_map>
#include <boost/algorithm/string.hpp>

namespace ankh::html{
  class html_visitor;
  namespace css3{ struct selector_visitor;}

  class html_element {
  public:
    enum ELEMENT_TYPE {
      ET_NORMAL,
      ET_SCRIPT,
      ET_COMMENT,
      ET_FOOTER,
      ET_DOCTYPE,
      ET_INNER_TEXT,
    };
    html_element(ELEMENT_TYPE type, std::string content);
    html_element(ELEMENT_TYPE type): type_(type){};
    html_element(std::string name, std::unique_ptr<std::unordered_map<std::string, std::string>> attributes, bool is_closed);
    ELEMENT_TYPE type();
    std::string to_string();
    std::string content();
    void set_parent(html_element* parent);
    void append_child(std::reference_wrapper<html_element> child);
    std::vector<std::reference_wrapper<html_element>> children();
    bool checked();
    bool can_be_disabled();
    bool disabled();
    bool closed() const;
    void set_closed(bool closed);
    void descendants(std::unordered_map<html_element*,std::reference_wrapper<html_element>>& result);
  protected:
    std::string content_;
    ELEMENT_TYPE type_;
    std::unique_ptr<std::unordered_map<std::string, bool>> classes_;
    std::unique_ptr<std::unordered_map<std::string,std::string>> attributes_;
    std::unique_ptr<std::vector<std::reference_wrapper<html_element>>> children_;
    html_element* parent_ = nullptr;
    bool is_closed_ = true;

    friend class ankh::html::html_visitor;
    friend class ankh::html::css3::selector_visitor;
  };




}


#endif //ZK_CPP_SPIDER_HTML_ELEMENT_H
