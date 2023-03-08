//
// Created by Hackman.Lo on 2/17/2023.
//

#ifndef ZK_CPP_SPIDER_CSS3_SELECTOR_H
#define ZK_CPP_SPIDER_CSS3_SELECTOR_H
#include <string>
#include <variant>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/optional.hpp>
#include <boost/fusion/support/pair.hpp>

namespace ankh::ast {
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
    bool v_;
  };

  struct first_of_type{
    bool v_;
  };
  struct last_of_type{
    bool v_;
  };
  struct only_of_type{
    bool v_;
  };
  struct empty_type{
    bool v_;
  };
  struct enabled{
    bool enabled_;
  };
  struct checked{
    bool checked_;
  };


  using css_single_element = boost::variant<html_selector, class_selector, id_selector, selector_operator,
  attribute_selector, attribute_exists_selector, nth_child_constant, nth_child_odd_even, nth_child_variant,
  last_nth_child_constant, last_nth_child_odd_even, last_nth_child_variant,
  last_child, first_of_type, last_of_type, only_of_type, empty_type, enabled, checked
  >;

//  struct selector_group{
//    std::vector<css_single_element>
//  };

  using boost::fusion::operator <<;
}

BOOST_FUSION_ADAPT_STRUCT(ankh::ast::html_selector, tag_);
BOOST_FUSION_ADAPT_STRUCT(ankh::ast::class_selector, name_);
BOOST_FUSION_ADAPT_STRUCT(ankh::ast::id_selector, id_);
BOOST_FUSION_ADAPT_STRUCT(ankh::ast::selector_operator, operator_);
BOOST_FUSION_ADAPT_STRUCT(ankh::ast::attribute_exists_selector, attr_name_);
BOOST_FUSION_ADAPT_STRUCT(ankh::ast::attribute_selector, attr_name_, operator_, attr_value_)
BOOST_FUSION_ADAPT_STRUCT(ankh::ast::nth_child_variant::optional_data, operator_, additional_data_)
BOOST_FUSION_ADAPT_STRUCT(ankh::ast::nth_child_variant, factor_, optional_)
BOOST_FUSION_ADAPT_STRUCT(ankh::ast::nth_child_odd_even, odd_)
BOOST_FUSION_ADAPT_STRUCT(ankh::ast::nth_child_constant, nth_)
BOOST_FUSION_ADAPT_STRUCT(ankh::ast::last_nth_child_variant::optional_data, operator_, additional_data_)
BOOST_FUSION_ADAPT_STRUCT(ankh::ast::last_nth_child_variant, factor_, optional_)
BOOST_FUSION_ADAPT_STRUCT(ankh::ast::last_nth_child_odd_even, odd_)
BOOST_FUSION_ADAPT_STRUCT(ankh::ast::last_nth_child_constant, nth_)
BOOST_FUSION_ADAPT_STRUCT(ankh::ast::last_child, v_)
BOOST_FUSION_ADAPT_STRUCT(ankh::ast::first_of_type, v_)
BOOST_FUSION_ADAPT_STRUCT(ankh::ast::last_of_type, v_)
BOOST_FUSION_ADAPT_STRUCT(ankh::ast::only_of_type, v_)
BOOST_FUSION_ADAPT_STRUCT(ankh::ast::empty_type, v_)
BOOST_FUSION_ADAPT_STRUCT(ankh::ast::enabled, enabled_)
BOOST_FUSION_ADAPT_STRUCT(ankh::ast::checked, checked_)
#endif //ZK_CPP_SPIDER_CSS3_SELECTOR_H
