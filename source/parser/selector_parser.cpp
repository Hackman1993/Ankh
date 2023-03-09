#include "constructure/html_element.h"
#include "parser/html_parser.h"
#include "parser/selector_visitor.h"
using namespace boost::spirit::x3;
namespace ankh::html::css3::parser {
  struct selector_parser_id: html::common::error_handler{};
  auto html_selector_parser_ = rule < selector_parser_id, ast::html_selector>{"ElementSelectorParser"} = (+~char_(" [>.,:~)"));
  auto class_selector_parser_ = rule<selector_parser_id, ast::class_selector>{"ClassSelectorParser"} = '.' >> *~char_(" [>.,:~)");
  auto id_selector_parser_ = rule<selector_parser_id, ast::id_selector>{"IDSelectorParser"} = '#' >> *~char_(" [>.,:~)");


  auto nth_child_selector_constant = rule<selector_parser_id, ast::nth_child_constant>{"NthChildConstantSelectorParser"} = (":nth-child(" >> uint_ >> ")");
  auto nth_child_selector_oddeven = rule<selector_parser_id, ast::nth_child_odd_even>{"NthChildOddEvenSelectorParser"} = (":nth-child(" >> (("odd" >> attr(1)) | ("even" >> attr(0))) >> ")");
  auto nth_child_selector_variant = rule<selector_parser_id, ast::nth_child_variant>{"VariantChildSelectorParser"} = (":nth-child(" > -uint_ > "n" >> -(char_("+-") > uint_) >> ")");
  auto nth_child_selector_parser_ = rule<selector_parser_id, boost::variant<ast::nth_child_constant,ast::nth_child_odd_even,ast::nth_child_variant>>{"NthChildSelectorParser"} = nth_child_selector_oddeven | nth_child_selector_constant | nth_child_selector_variant;

  auto last_nth_child_selector_constant = rule<selector_parser_id, ast::last_nth_child_constant>{"LastNthChildConstantSelectorParser"} = (":nth-last-child(" >> uint_ >> ")");
  auto last_nth_child_selector_oddeven = rule<selector_parser_id, ast::last_nth_child_odd_even>{"LastNthChildOddEvenSelectorParser"} = (":nth-last-child(" >> (("odd" >> attr(1)) | ("even" >> attr(0))) >> ")");
  auto last_nth_child_selector_variant = rule<selector_parser_id, ast::last_nth_child_variant>{"LastVariantChildSelectorParser"} = (":nth-last-child(" > -uint_ > "n" >> -(char_("+-") > uint_) >> ")");
  auto last_nth_child_selector_parser_ = rule<selector_parser_id, boost::variant<ast::last_nth_child_constant,ast::last_nth_child_odd_even,ast::last_nth_child_variant>>{"NthChildSelectorParser"} = last_nth_child_selector_oddeven | last_nth_child_selector_constant | last_nth_child_selector_variant;

  auto last_child_selector_parser_ = rule<selector_parser_id, ast::last_child>{"LastChildSelectorParser"} = lit(":last-child") >> attr(ast::last_child{});
  auto first_of_type_selector_parser_ = rule<selector_parser_id, ast::first_of_type>{"FirstOfTypeSelectorParser"} = lit(":first-of-type") >> attr(ast::first_of_type{});
  auto last_of_type_selector_parser_ = rule<selector_parser_id, ast::last_of_type>{"LastOfTypeSelectorParser"} = lit(":last-of-type") >> attr(ast::last_of_type{});
  auto only_of_type_selector_parser_ = rule<selector_parser_id, ast::only_of_type>{"OnlyOfTypeSelectorParser"} = lit(":only-of-type") >> attr(ast::only_of_type{});
  auto empty_selector_parser_ = rule<selector_parser_id, ast::empty_type>{"EmptySelectorParser"} = lit(":empty") >> attr(ast::empty_type{});

  auto enabled_selector_parser_ = rule<selector_parser_id, ast::enabled>{"EnableOrDisableSelectorParser"} = (lit(":enabled") >> attr(true)) | (lit(":disabled") >> attr(false));
  auto checked_selector_parser_ = rule<selector_parser_id, ast::checked>{"CheckedSelectorParser"} = lit(":checked") >> attr(true);


  auto operator_parser_ = rule<selector_parser_id, ast::selector_operator>{"OperatorParser"} =
                              // + Operator
                              omit[*(char_(' '))>>char_('+')>> *(char_(' '))] >> attr('+')|
                              // , Operator
                              omit[*(char_(' '))>>char_(',')>> *(char_(' '))] >> attr(',')|
                              // > Operator
                              omit[*(char_(' '))>>char_('>')>> *(char_(' '))] >> attr('>')|
                              // ~ Operator
                              omit[*(char_(' '))>>char_('~')>> *(char_(' '))] >> attr('~')|
                              // Space Operator
                              omit[+char_(' ')]>>attr(' ')
  ;

  auto attribute_exists_parser_ = rule<selector_parser_id, ast::attribute_exists_selector>{"AttributeExistsSelectorParser"} = ("[" >> +~char_("^$*=]")) >> ']';
  auto attribute_selector_parser_ = rule<selector_parser_id, ast::attribute_selector >{"AttributeSelectorParser"} = '[' > lexeme[+~char_("^=*$")] > -char_("^$*") > "=" > "'" > lexeme[+~char_("'")] > "'" > "]";


  rule<selector_parser_id, ast::not_> not_parser_ = "NotParser";
  rule<selector_parser_id, ast::css_single_element> single_selector_parser_ = "SingleSelectorParser";
  //rule<selector_parser_id, std::vector<ast::css_single_element>> css3_selector_group_parser_ = "SelectorGroupParser";
  rule<selector_parser_id, std::vector<ast::css_single_element>> css3_selector_parser_ = "CSS3Parser";

  auto not_parser__def = ":not(" > css3_selector_parser_ > ")";
  auto single_selector_parser__def =  not_parser_|checked_selector_parser_ | enabled_selector_parser_ | only_of_type_selector_parser_ | empty_selector_parser_ | last_of_type_selector_parser_ | first_of_type_selector_parser_ | last_child_selector_parser_ | last_nth_child_selector_parser_ | nth_child_selector_parser_ | attribute_exists_parser_ | attribute_selector_parser_ | class_selector_parser_ | html_selector_parser_ | id_selector_parser_;
  auto css3_selector_parser__def = boost::spirit::x3::no_skip[*single_selector_parser_ >> *(operator_parser_ > single_selector_parser_)];
  //auto  = css3_selector_group_parser_];
  BOOST_SPIRIT_DEFINE(not_parser_, single_selector_parser_, css3_selector_parser_);


  std::vector<ast::css_single_element> parse_ast(std::string source) {
    using error_handler_type = boost::spirit::x3::error_handler<std::string::const_iterator>;
    error_handler_type error_handler(source.begin(), source.end(), std::cerr);
    std::vector<ast::css_single_element> css_ast;
    auto const parser = boost::spirit::x3::with<boost::spirit::x3::error_handler_tag>(std::ref(error_handler))[css3_selector_parser_];
    auto parse_result = boost::spirit::x3::phrase_parse(source.begin(), source.end(), parser, boost::spirit::x3::ascii::space, css_ast);
    if (parse_result)
      return css_ast;
    return {};
  }

  std::unordered_map<html_element *, std::reference_wrapper<html_element>> execute_query_(const std::vector<html::css3::ast::css_single_element> &css_ast, const std::unordered_map<html_element *, std::reference_wrapper<html_element>> &input_elements) {
    std::unordered_map<html_element *, std::reference_wrapper<html_element>> final_result;
    bool use_last_result = false;
    std::unordered_map<html_element *, std::reference_wrapper<html_element>> last_result;
    for (auto &selector: css_ast) {
      std::unordered_map<html_element *, std::reference_wrapper<html_element>> next_input = use_last_result ? last_result : input_elements;
      css3::selector_visitor visitor(next_input, input_elements);
      auto helper = selector.apply_visitor(visitor);
      if (helper.final) {
        for (auto &res: helper.result_) {
          final_result.emplace(res);
        }
        helper.result_.clear();
        use_last_result = false;
      } else {
        use_last_result = true;
      }
      last_result = std::move(helper.result_);
    }
    for (auto &fin_data: last_result) {
      final_result.emplace(fin_data);
    }
    return final_result;
  }

  std::unordered_map<html_element*,std::reference_wrapper<html_element>> execute_query_m(std::string source, const std::unordered_map<html_element*, std::unique_ptr<html_element>>& elements_)
  {
    auto css_ast = parse_ast(std::move(source));
    // Preparing Input Elements
    std::unordered_map<html_element*,std::reference_wrapper<html_element>> input_elements;
    for(auto &item : elements_){ input_elements.emplace(item.first, *item.second); }

    return execute_query_(css_ast, input_elements);
  }

  std::vector<std::reference_wrapper<html_element>> execute_query_v(std::string source, const std::unordered_map<html_element*, std::unique_ptr<html_element>>& elements_)
  {
    auto result = execute_query_m(std::move(source), elements_);

    std::vector<std::reference_wrapper<html_element>> res;
    for(auto& fin_res: result){
      res.emplace_back(fin_res.second);
    }
    return res;

  }
}