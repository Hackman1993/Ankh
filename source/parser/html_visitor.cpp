//
// Created by Hackman.Lo on 3/9/2023.
//

#include <memory>
#include "ast/html.h"
#include "parser/html_visitor.h"
#include <boost/algorithm/string.hpp>
#include "constructure/html_element.h"

namespace ankh::html::parser {

    html_visitor::result_type html_visitor::operator()(const ast::tag_header &data) {
      std::unique_ptr < std::unordered_map < std::string, std::string >> attributes = std::make_unique < std::unordered_map < std::string, std::string >> ();
      for (const auto &attribute: data.attributes) {
        std::string lower_name = attribute.name;
        std::string lower_value = attribute.value.get_value_or("true");
        boost::algorithm::to_lower(lower_name);
        boost::algorithm::to_lower(lower_value);
        attributes->emplace(lower_name, lower_value);
      }
      return std::make_unique<html_element>(data.name, std::move(attributes), data.closed);
    }

    html_visitor::result_type html_visitor::operator()(const ast::tag_footer &data) const {
      return std::make_unique<html_element>(html_element::ET_FOOTER, data.name);
    }

    html_visitor::result_type html_visitor::operator()(const std::string &data) const {
      return std::make_unique<html::html_element>(html_element::ET_INNER_TEXT, data);;
    }

    html_visitor::result_type html_visitor::operator()(const ast::comment_tag &data) const {
      return std::make_unique<html_element>(html_element::ET_COMMENT, data.content);
    }

    html_visitor::result_type html_visitor::operator()(const ast::doctype_tag &data) const {
      return std::make_unique<html_element>(html_element::ET_DOCTYPE, data.content);
    }

    html_visitor::result_type html_visitor::operator()(const ast::script_tag &data) const {
      std::unique_ptr < std::unordered_map < std::string, std::string >> attributes = std::make_unique < std::unordered_map < std::string, std::string >> ();
      for (const auto &attribute: data.attributes_) {
        std::string lower_name = attribute.name;
        std::string lower_value = attribute.value.get_value_or("true");
        boost::algorithm::to_lower(lower_name);
        boost::algorithm::to_lower(lower_value);
        attributes->emplace(lower_name, lower_value);
      }
      return std::make_unique<html_element>("script", std::move(attributes), true);
    }
}
