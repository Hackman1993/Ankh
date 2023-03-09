//
// Created by Hackman.Lo on 3/9/2023.
//
#include <stack>
#include "parser/html_parser.h"
#include "parser/html_visitor.h"
#include "constructure/html_element.h"
namespace ankh::html::parser {
  using namespace boost::spirit::x3;
  struct tag_parser_id: html::common::error_handler{};

  /*********************** Attribute Parsers ****************************/
  auto attribute_identifier_= rule<tag_parser_id, std::string>{"AttributeIdentifier"} = lexeme[+(~char_("< /=>"))];
  auto attribute_value_= rule<tag_parser_id, std::string>{"AttributeValue"} = lexeme["\"" > *(~char_("\"")) > "\""]|lexeme["'" > *(~char_("'")) > "'"]| lexeme[+(~char_(" />"))];
  auto single_attribute_ = rule<tag_parser_id, ast::attribute_data>{"SingleAttribute"} = attribute_identifier_ > -("=" > attribute_value_);
  auto attributes_ = rule<tag_parser_id, std::vector<ast::attribute_data>>{"Attributes"} = (*single_attribute_);


  /*********************** Element Parsers ****************************/
  auto inner_text = lexeme[+(~char_("<"))];
  auto tag_name_ = rule<tag_parser_id, std::string>{"HtmlTagName"} = lexeme[*(~char_("< />"))];
  auto comment_ = rule<tag_parser_id,ast::comment_tag>{"CommentTag"} = "<!--" >> lexeme[*(char_ - "-->")] >> "-->";
  auto doctype_tag_ = rule<tag_parser_id,ast::doctype_tag>{"DoctypeTag"} = no_case["<!doctype"] >> lexeme[*~char_(">")] >> ">";
  auto script_tag_ = rule<tag_parser_id, ast::script_tag>{"ScriptTag"} = no_case["<script"] >> attributes_ >> ">" >> *(char_ - no_case["</script>"]) >> no_case["</script>"];
  auto tag_header_ = rule<tag_parser_id, ast::tag_header>{"HtmlTagHeader"} = '<' > tag_name_ > attributes_ > -('/' >> attr(true)) > '>';
  auto tag_footer_ = rule<tag_parser_id, ast::tag_footer>{"HtmlTagFooter"} = "</" > tag_name_ > '>';
  auto html_element_ = rule<tag_parser_id, ast::html_element>{"HtmlElement"} = inner_text | comment_ | doctype_tag_ | script_tag_  | tag_footer_ | tag_header_  ;

  std::vector<ast::html_element> parse_source(std::string &source) {
    std::vector<ast::html_element> result;
    using error_handler_type = boost::spirit::x3::error_handler<std::string::const_iterator>;
    error_handler_type error_handler(source.begin(), source.end(), std::cerr);
    auto const parser = boost::spirit::x3::with<boost::spirit::x3::error_handler_tag>(std::ref(error_handler))[html_element_];
    auto parse_result = phrase_parse(source.begin(), source.end(), *parser, boost::spirit::x3::ascii::space, result);
    if (parse_result)
      return result;
    return {};
  }

  bool parse_ast(const std::vector<ast::html_element> &html_ast, std::unordered_map<html_element*, std::unique_ptr<html_element>>& elements_, std::vector<std::reference_wrapper<html_element>>& structure_) {
    std::stack<html_element *> element_stack;
    for (auto ast: html_ast) {
      html_element *current_parent = element_stack.empty() ? nullptr : element_stack.top();
      html_visitor visitor;
      std::unique_ptr<html_element> result = ast.apply_visitor(visitor);
      switch (result->type()) {
        case html_element::ET_FOOTER:
          // if it's a close tag
          if (!current_parent) continue;
          if (current_parent->content() == result->content()) {
            current_parent->set_closed(true);
            element_stack.pop();
          } else {
            throw std::logic_error(std::string("Unexpected close tag of ") + result->content());
          }
          break;
        case html_element::ET_NORMAL:
          if (!result->closed()) {
            // If this tag is not closed , push this tag to the stack
            element_stack.push(result.get());
          }
        case html_element::ET_INNER_TEXT:
        case html_element::ET_COMMENT:
        case html_element::ET_SCRIPT:
          result->set_parent(current_parent);
          if (current_parent) {
            current_parent->append_child(*result);
          } else {
            structure_.emplace_back(*result);
          }
          elements_.emplace(result.get(), std::move(result));
      }
    }
    return true;
  }
}