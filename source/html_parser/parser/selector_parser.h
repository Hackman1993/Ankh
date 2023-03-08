//
// Created by Hackman.Lo on 2/17/2023.
//

#ifndef ZK_CPP_SPIDER_SELECTOR_PARSER_H
#define ZK_CPP_SPIDER_SELECTOR_PARSER_H

#include "common.h"
#include "../ast/css3_selector.h"
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#define BOOST_SPIRIT_X3_DEBUG
using namespace boost::spirit::x3;


namespace ankh::parser::css3{
  struct selector_parser_id: ankh::parser::common::error_handler{};

  auto html_selector_parser_ = rule < selector_parser_id, ankh::ast::html_selector>{"ElementSelectorParser"} = (+~char_(" [>.,:~"));
  auto class_selector_parser_ = rule<selector_parser_id, ankh::ast::class_selector>{"ClassSelectorParser"} = '.' >> *~char_(" [>.,:~");
  auto id_selector_parser_ = rule<selector_parser_id, ankh::ast::id_selector>{"IDSelectorParser"} = '#' >> *~char_(" [>.,:~");


  auto nth_child_selector_constant = rule<selector_parser_id, ankh::ast::nth_child_constant>{"NthChildConstantSelectorParser"} = (":nth-child(" >> uint_ >> ")");
  auto nth_child_selector_oddeven = rule<selector_parser_id, ankh::ast::nth_child_odd_even>{"NthChildOddEvenSelectorParser"} = (":nth-child(" >> (("odd" >> attr(1)) | ("even" >> attr(0))) >> ")");
  auto nth_child_selector_variant = rule<selector_parser_id, ankh::ast::nth_child_variant>{"VariantChildSelectorParser"} = (":nth-child(" > -uint_ > "n" >> -(char_("+-") > uint_) >> ")");
  auto nth_child_selector_parser_ = rule<selector_parser_id, boost::variant<ankh::ast::nth_child_constant,ankh::ast::nth_child_odd_even,ankh::ast::nth_child_variant>>{"NthChildSelectorParser"} = nth_child_selector_oddeven | nth_child_selector_constant | nth_child_selector_variant;

  auto last_nth_child_selector_constant = rule<selector_parser_id, ankh::ast::last_nth_child_constant>{"LastNthChildConstantSelectorParser"} = (":nth-last-child(" >> uint_ >> ")");
  auto last_nth_child_selector_oddeven = rule<selector_parser_id, ankh::ast::last_nth_child_odd_even>{"LastNthChildOddEvenSelectorParser"} = (":nth-last-child(" >> (("odd" >> attr(1)) | ("even" >> attr(0))) >> ")");
  auto last_nth_child_selector_variant = rule<selector_parser_id, ankh::ast::last_nth_child_variant>{"LastVariantChildSelectorParser"} = (":nth-last-child(" > -uint_ > "n" >> -(char_("+-") > uint_) >> ")");
  auto last_nth_child_selector_parser_ = rule<selector_parser_id, boost::variant<ankh::ast::last_nth_child_constant,ankh::ast::last_nth_child_odd_even,ankh::ast::last_nth_child_variant>>{"NthChildSelectorParser"} = last_nth_child_selector_oddeven | last_nth_child_selector_constant | last_nth_child_selector_variant;

  auto last_child_selector_parser_ = rule<selector_parser_id, ankh::ast::last_child>{"LastChildSelectorParser"} = lit(":last-child") >> attr(true);

  auto first_of_type_selector_parser_ = rule<selector_parser_id, ankh::ast::first_of_type>{"FirstOfTypeSelectorParser"} = lit(":first-of-type") >> attr(true);
  auto last_of_type_selector_parser_ = rule<selector_parser_id, ankh::ast::last_of_type>{"LastOfTypeSelectorParser"} = lit(":last-of-type") >> attr(true);
  auto only_of_type_selector_parser_ = rule<selector_parser_id, ankh::ast::only_of_type>{"OnlyOfTypeSelectorParser"} = lit(":only-of-type") >> attr(true);

  auto empty_selector_parser_ = rule<selector_parser_id, ankh::ast::empty_type>{"EmptySelectorParser"} = lit(":empty") >> attr(true);
  auto enabled_selector_parser_ = rule<selector_parser_id, ankh::ast::enabled>{"EnableOrDisableSelectorParser"} = (lit(":enabled") >> attr(true)) | (lit(":disabled") >> attr(false));
  auto checked_selector_parser_ = rule<selector_parser_id, ankh::ast::checked>{"CheckedSelectorParser"} = lit(":checked") >> attr(true);


  auto operator_parser_ = rule<selector_parser_id, ankh::ast::selector_operator>{"OperatorParser"} =
                              // + 操作符
                              omit[*(char_(' '))>>char_('+')>> *(char_(' '))] >> attr('+')|
                              // , 操作符
                              omit[*(char_(' '))>>char_(',')>> *(char_(' '))] >> attr(',')|
                              // > 操作符
                              omit[*(char_(' '))>>char_('>')>> *(char_(' '))] >> attr('>')|
                              // ~ 操作符
                              omit[*(char_(' '))>>char_('~')>> *(char_(' '))] >> attr('~')|
                              // 空格操作符
                              omit[+char_(' ')]>>attr(' ')
      ;

  auto attribute_exists_parser_ = rule<selector_parser_id, ankh::ast::attribute_exists_selector>{"AttributeExistsSelectorParser"} = ("[" >> +~char_("^$*=]")) >> ']';
  auto attribute_selector_parser_ = rule<selector_parser_id, ankh::ast::attribute_selector >{"AttributeSelectorParser"} = '[' > lexeme[+~char_("^=*$")] > -char_("^$*") > "=" > "'" > lexeme[+~char_("'")] > "'" > "]";


  auto single_selector_parser_ = rule<selector_parser_id, ankh::ast::css_single_element>{"SingleSelectorParser"} = checked_selector_parser_ | enabled_selector_parser_ | only_of_type_selector_parser_ | empty_selector_parser_ | last_of_type_selector_parser_ | first_of_type_selector_parser_ | last_child_selector_parser_ | last_nth_child_selector_parser_ | nth_child_selector_parser_ | attribute_exists_parser_ | attribute_selector_parser_ | class_selector_parser_ | html_selector_parser_ | id_selector_parser_;
  //auto multi_selector = rule<selector_parser_id, ankh::ast::
  auto css3_selector_group_parser_ = rule<selector_parser_id, std::vector<ankh::ast::css_single_element>>{"SelectorGroupParser"} = *single_selector_parser_ >> *(operator_parser_ > single_selector_parser_);

  auto css3_selector_parser_ = rule<selector_parser_id, std::vector<ankh::ast::css_single_element>>{"CSS3Parser"} = css3_selector_group_parser_;
}



#endif //ZK_CPP_SPIDER_SELECTOR_PARSER_H
