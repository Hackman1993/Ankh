//
// Created by Hackman.Lo on 11/20/2022.
//

#ifndef ZK_CPP_SPIDER_TAG_H
#define ZK_CPP_SPIDER_TAG_H

#include <string>
#include <boost/optional.hpp>
#include "attribute.h"
#include <utility>
#include <vector>
#include <boost/fusion/include/adapt_struct.hpp>
namespace ankh::ast{
  struct script_tag : boost::spirit::x3::position_tagged{
    std::vector<attribute_data> attributes_;
    std::string content_;
  };
  struct tag_header :  boost::spirit::x3::position_tagged {
    std::string name;
    std::vector<attribute_data> attributes;
    bool closed = false;
  };

  struct tag_footer : boost::spirit::x3::position_tagged{
    std::string name;
  };

  struct doctype_tag : boost::spirit::x3::position_tagged{
    std::string content;
  };

  struct comment_tag: boost::spirit::x3::position_tagged{
    std::string content;
  };


  using html_element = boost::variant<comment_tag, std::string, tag_header, tag_footer, script_tag, doctype_tag>;
//  struct html_element : boost::spirit::x3::position_tagged, boost::spirit::x3::variant< std::string, tag_header, tag_footer>{
//    using base_type::base_type;
//    using base_type::operator=;
//  };

  using boost::fusion::operator<<;
}

BOOST_FUSION_ADAPT_STRUCT(ankh::ast::tag_header, name, attributes, closed);
BOOST_FUSION_ADAPT_STRUCT(ankh::ast::script_tag, attributes_, content_);
BOOST_FUSION_ADAPT_STRUCT(ankh::ast::tag_footer, name);
BOOST_FUSION_ADAPT_STRUCT(ankh::ast::comment_tag, content);
BOOST_FUSION_ADAPT_STRUCT(ankh::ast::doctype_tag, content);

#endif //ZK_CPP_SPIDER_TAG_H
