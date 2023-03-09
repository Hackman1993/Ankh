//
// Created by Hackman.Lo on 2/17/2023.
//

#ifndef ZK_CPP_SPIDER_SELECTOR_H
#define ZK_CPP_SPIDER_SELECTOR_H
#include <string>
#include <variant>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/optional.hpp>
#include <boost/fusion/support/pair.hpp>

namespace ankh::html::css3::ast {
  struct class_selector {
    std::string name_;
  };

  struct id_selector {
    std::string id_;
  };

  struct html_selector{
    std::string tag_;
  };
  struct selector_operator{
    char operator_;
  };


  struct attribute_exists_selector{
    std::string attr_name_;
  };

  struct attribute_selector{
    std::string attr_name_;
    char operator_ = 0;
    std::string attr_value_;
  };


  struct nth_child_constant{
    unsigned int nth_;
  };
  struct nth_child_odd_even{
    unsigned int odd_;
  };
  struct nth_child_variant{
    unsigned int factor_ = 1;
    struct optional_data{
      char operator_ = '+';
      unsigned int additional_data_ = 0;
    };
    boost::optional<optional_data> optional_;
  };

  struct last_nth_child_constant{
    unsigned int nth_;
  };
  struct last_nth_child_odd_even{
    unsigned int odd_;
  };
  struct last_nth_child_variant{
    unsigned int factor_ = 1;
    struct optional_data{
      char operator_ = '+';
      unsigned int additional_data_ = 0;
    };
    boost::optional<optional_data> optional_;
  };

  struct last_child{
    //[[maybe_unused]] bool v_;
  };

  struct first_of_type{
    //[[maybe_unused]] bool v_;
  };
  struct last_of_type{
    // [[maybe_unused]] bool v_;
  };
  struct only_of_type{
    //[[maybe_unused]] bool v_;
  };
  struct empty_type{
    //[[maybe_unused]] bool v_;
  };
  struct enabled{
    bool enabled_;
  };
  struct checked{
    bool checked_;
  };


  struct not_;


  using css_single_element = boost::spirit::x3::variant<html_selector, class_selector, id_selector, selector_operator,
  attribute_selector, attribute_exists_selector, nth_child_constant, nth_child_odd_even, nth_child_variant,
  last_nth_child_constant, last_nth_child_odd_even, last_nth_child_variant,
  last_child, first_of_type, last_of_type, only_of_type, empty_type, enabled, checked, boost::spirit::x3::forward_ast<not_>
  >;

  struct not_{
    std::vector<css_single_element> queries_;
  };

  using boost::fusion::operator <<;
}

BOOST_FUSION_ADAPT_STRUCT(ankh::html::css3::ast::html_selector, tag_);
BOOST_FUSION_ADAPT_STRUCT(ankh::html::css3::ast::class_selector, name_);
BOOST_FUSION_ADAPT_STRUCT(ankh::html::css3::ast::id_selector, id_);
BOOST_FUSION_ADAPT_STRUCT(ankh::html::css3::ast::selector_operator, operator_);
BOOST_FUSION_ADAPT_STRUCT(ankh::html::css3::ast::attribute_exists_selector, attr_name_);
BOOST_FUSION_ADAPT_STRUCT(ankh::html::css3::ast::attribute_selector, attr_name_, operator_, attr_value_)
BOOST_FUSION_ADAPT_STRUCT(ankh::html::css3::ast::nth_child_variant::optional_data, operator_, additional_data_)
BOOST_FUSION_ADAPT_STRUCT(ankh::html::css3::ast::nth_child_variant, factor_, optional_)
BOOST_FUSION_ADAPT_STRUCT(ankh::html::css3::ast::nth_child_odd_even, odd_)
BOOST_FUSION_ADAPT_STRUCT(ankh::html::css3::ast::nth_child_constant, nth_)
BOOST_FUSION_ADAPT_STRUCT(ankh::html::css3::ast::last_nth_child_variant::optional_data, operator_, additional_data_)
BOOST_FUSION_ADAPT_STRUCT(ankh::html::css3::ast::last_nth_child_variant, factor_, optional_)
BOOST_FUSION_ADAPT_STRUCT(ankh::html::css3::ast::last_nth_child_odd_even, odd_)
BOOST_FUSION_ADAPT_STRUCT(ankh::html::css3::ast::last_nth_child_constant, nth_)
//BOOST_FUSION_ADAPT_STRUCT(ankh::html::css3::ast::last_child, v_)
//BOOST_FUSION_ADAPT_STRUCT(ankh::html::css3::ast::first_of_type, v_)
//BOOST_FUSION_ADAPT_STRUCT(ankh::html::css3::ast::last_of_type, v_)
//BOOST_FUSION_ADAPT_STRUCT(ankh::html::css3::ast::only_of_type, v_)
//BOOST_FUSION_ADAPT_STRUCT(ankh::html::css3::ast::empty_type, v_)
BOOST_FUSION_ADAPT_STRUCT(ankh::html::css3::ast::enabled, enabled_)
BOOST_FUSION_ADAPT_STRUCT(ankh::html::css3::ast::checked, checked_)
BOOST_FUSION_ADAPT_STRUCT(ankh::html::css3::ast::not_, queries_)
#endif //ZK_CPP_SPIDER_SELECTOR_H
