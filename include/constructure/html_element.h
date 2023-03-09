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

    html_element(ELEMENT_TYPE type, std::string content): type_(type), content_(std::move(content)) {
      if(type_ == ET_NORMAL || type == ET_FOOTER) boost::algorithm::to_lower(content_);
    }
    html_element(ELEMENT_TYPE type): type_(type){}
    html_element(std::string name, std::unique_ptr<std::unordered_map<std::string, std::string>> attributes, bool is_closed):
        attributes_(std::move(attributes)), is_closed_(is_closed), type_(ET_NORMAL)
    {
      // 所有标签转换为小写
      boost::algorithm::to_lower(name);
      content_ = std::move(name);
      if(attributes_&& attributes_->contains("class")) {
        std::vector<std::string> classes;
        boost::algorithm::split(classes,(*attributes_)["class"],boost::is_any_of(" "));
        for(auto &class_: classes){
          if(!classes_) classes_ = std::make_unique<std::unordered_map<std::string, bool>>();
          classes_->emplace(class_, 1);
        }
      }
      if(content_ == "link" || content_ == "meta" || content_ == "img"||
       content_ == "input" || content_ == "br" || content_ == "area"||
       content_ == "col" || content_ == "embed" || content_ == "base" ||
       content_ == "hr" || content_=="param" || content_ == "source" ||
       content_ == "track" || content_ == "wbr"){
        is_closed_ = true;
      }
    }


    ~html_element(){
    }

    ELEMENT_TYPE type(){
      return type_;
    }

    std::string to_string(){
      std::string result;
      std::string attribute;
      if(attributes_){
        for(auto &attr : *attributes_){
          attribute += " " + attr.first + "=\"" + attr.second + "\"";
        }
      }
      switch (type_) {
        case ET_SCRIPT:
          result = "<script" + attribute + ">" + content_ + "</script>";
          break;
        case ET_FOOTER:
          result = "</" + content_ +">";
          break;
        case ET_DOCTYPE:
          result = "<!DOCTYPE " + content_ + ">";
          break;
        case ET_COMMENT:
          result = "<!--" + content_ + "-->";
          break;
        case ET_NORMAL:
          result = "<" + content_ + attribute + (children_? ">" : "/>");
          if(children_)
          {
            for(auto &child: *children_){
              result += child.get().to_string();
            }
            result += "</" + content_ + ">";
          }
          break;
        case ET_INNER_TEXT:
          result = content_;
          break;
      }
      return result;
    }
    std::string content(){
      return content_;
    }

    void set_parent(html_element* parent){
      parent_ = parent;
    }

    void append_child(std::reference_wrapper<html_element> child){
      if(!children_)
        children_ = std::make_unique<std::vector<std::reference_wrapper<html_element>>>();
      children_->emplace_back(child);
    }

    std::vector<std::reference_wrapper<html_element>> children(){
      return children_? *children_:std::vector<std::reference_wrapper<html_element>>();
    }

    bool checked()
    {
      if(type_!=ET_NORMAL || content_!="input" || !attributes_ || boost::algorithm::trim_copy((*attributes_)["type"])!="checkbox")
        return false;
      return attributes_->contains("checked");
    }

    bool can_be_disabled()
    {
      std::vector<std::string> str_list={"input", "button", "select", "textarea", "optgroup", "option", "fieldset"};
      // Type can't be disabled , directly return false
      if(type_ != ET_NORMAL || !std::ranges::any_of(str_list, [&](const std::string& str){ return str == content_;}))
        return false;
      return true;
    }

    bool disabled(){
      if(!can_be_disabled())
        return false;
      if(attributes_->contains("disabled")) return true;
      // If type can be disabled but not disabled by attributes, then we need to find a parent element which can be disabled and be disabled
      if(parent_ && parent_->disabled())
        return true;
      return false;
    }

    [[nodiscard]] bool closed() const{
      return is_closed_;
    }

    void set_closed(bool closed){
      is_closed_ = closed;
    }
    void descendants(std::unordered_map<html_element*,std::reference_wrapper<html_element>>& result){
      if(!children_) return;
      for(auto & child: *children_){
        result.emplace(&child.get(), child);
        child.get().descendants(result);
      }
    }
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
