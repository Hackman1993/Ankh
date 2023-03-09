//
// Created by Hackman.Lo on 2/21/2023.
//

#ifndef ZK_CPP_SPIDER_SELECTOR_VISITOR_H
#define ZK_CPP_SPIDER_SELECTOR_VISITOR_H
#include <utility>
#include <vector>
#include <ranges>
#include "../ast/selector.h"
#include "../constructure/html_element.h"
#include <boost/bind/bind.hpp>
#include <boost/range/adaptor/reversed.hpp>
namespace ankh::html{
  class html_element;
}
namespace ankh::html::css3{
  struct selector_visitor_result{
    std::unordered_map<html::html_element*,std::reference_wrapper<ankh::html::html_element>> result_;
    bool final = false;
  };

  struct selector_visitor {
    explicit selector_visitor(const std::unordered_map<html::html_element*, std::reference_wrapper<ankh::html::html_element>>& elements, const std::unordered_map<html::html_element*, std::reference_wrapper<ankh::html::html_element>>& base_input): inputs_(elements), base_input_(base_input){}
    using result_type = selector_visitor_result;
    const std::unordered_map<html::html_element*,std::reference_wrapper<ankh::html::html_element>>& inputs_;
    std::unordered_map<html::html_element*,std::reference_wrapper<ankh::html::html_element>> result_;

    const std::unordered_map<html::html_element*,std::reference_wrapper<ankh::html::html_element>>& base_input_;

    bool is_valid(ankh::html::html_element& element);
    // ID Selector Matcher
    result_type operator()( const ast::id_selector& selector);

    // Class Selector Matcher
    result_type operator()( const ast::class_selector& selector);

    // Element Selector Matcher
    result_type operator()( const ast::html_selector& selector);
    // NthChild Constant Selector Matcher
    result_type operator()(const ast::nth_child_constant& selector);
    // NthChild OddEven Selector Matcher
    result_type operator()(const ast::nth_child_odd_even& selector);
    // NthChild Variant Selector Matcher
    result_type operator()(const ast::nth_child_variant& selector);
    // NthLastChild Constant Selector Matcher
    result_type operator()(const ast::last_nth_child_constant& selector);
    // NthLastChild OddEven Selector Matcher
    result_type operator()(const ast::last_nth_child_odd_even& selector);
    // NthLastChild Variant Selector Matcher
    result_type operator()(const ast::last_nth_child_variant& selector);
    // LastChild Selector Matcher
    result_type operator()(const ast::last_child& selector);
    // FirstOfType Selector Matcher
    result_type operator()(const ast::first_of_type& selector);
    // LastOfType Selector Matcher
    result_type operator()(const ast::last_of_type& selector);
    // OnlyOfType Selector Matcher
    result_type operator()(const ast::only_of_type& selector);
    // Empty Selector Matcher
    result_type operator()(const ast::empty_type& selector);
    // Enabled Selector Matcher
    result_type operator()(const ast::enabled& selector);
    // Checked Selector Matcher
    result_type operator()(const ast::checked& selector);
    // Not Selector Matcher
    result_type operator()(const ast::not_& selector);
    // Attribute Exists Matcher
    result_type operator()( const ast::attribute_exists_selector& selector);
    // Attribute Selector Matcher
    result_type operator()( const ast::attribute_selector& selector);
    // Descendants Operator Matcher
    result_type operator()(const ast::selector_operator& selector);
  private:
    void append_result(html::html_element* pointer, std::reference_wrapper<ankh::html::html_element> element);
  };
}

#endif //ZK_CPP_SPIDER_SELECTOR_VISITOR_H
