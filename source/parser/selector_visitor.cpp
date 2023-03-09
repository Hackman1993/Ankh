#include "parser/selector_visitor.h"
namespace ankh::html::css3 {
  /*********************ID,Class,Element Selectors*********************/
  selector_visitor_result selector_visitor::operator ()(const ast::id_selector& selector) {
    for(auto &element: inputs_){
      if(is_valid(element.second) && element.second.get().attributes_ && (*element.second.get().attributes_)["id"] == selector.id_){
        append_result(element.first, element.second);
      }
    }
    return selector_visitor_result{std::move(result_), false};
  }
  selector_visitor_result selector_visitor::operator()(const ast::class_selector &selector) {
    for(auto &element: inputs_){
      if(is_valid(element.second) && element.second.get().classes_ && element.second.get().classes_->contains(selector.name_)){
        append_result(element.first, element.second);
      }
    }
    return selector_visitor_result{std::move(result_), false};
  }
  selector_visitor::result_type selector_visitor::operator()(const ast::html_selector &selector) {
    for(auto &element: inputs_){
      if(is_valid(element.second) && element.second.get().content_ == selector.tag_){
        append_result(element.first, element.second);
      }
    }
    return selector_visitor_result{std::move(result_), false};
  }

  /*********************Attribute Selectors*********************/
  selector_visitor_result selector_visitor::operator()(const ast::attribute_exists_selector &selector) {
    for(auto &element: inputs_){
      if(is_valid(element.second) && element.second.get().attributes_ && element.second.get().attributes_->contains(selector.attr_name_)) {
        append_result(element.first, element.second);
      }
    }
    return selector_visitor_result{std::move(result_), false};
  }
  selector_visitor_result selector_visitor::operator()(const ast::attribute_selector &selector) {
    for(auto &element: inputs_){
      if(is_valid(element.second) && element.second.get().attributes_ ){
        bool match = false;
        for(auto& attribute: *element.second.get().attributes_)
        {
          if((selector.operator_ == '*' && boost::algorithm::contains(attribute.second, selector.attr_value_))||
             (selector.operator_ == '^' && boost::algorithm::starts_with(attribute.second, selector.attr_value_))||
             (selector.operator_ == '$' && boost::algorithm::ends_with(attribute.second, selector.attr_value_))||
             (selector.operator_ == '\0' && attribute.second == selector.attr_value_))
          {
            match = true;
            break;
          }
        }
        if(match){
          append_result(element.first, element.second);
        }
      }
    }
    return selector_visitor_result{std::move(result_), false};
  }

  /*********************Pseudo-class Selectors*********************/
  /// NthChild Selectors
  selector_visitor::result_type selector_visitor::operator()(const ast::nth_child_variant &selector) {

    int additional_data_ = 0;
    if(selector.optional_.has_value())
    {
      additional_data_ = selector.optional_->operator_=='+' ? static_cast<int>(selector.optional_->additional_data_):static_cast<int>(selector.optional_->additional_data_)*-1;
    }
    std::unordered_map<html::html_element*, bool> parent_filter;
    for(auto &element: inputs_){
      if(!is_valid(element.second.get())||!element.second.get().parent_||!element.second.get().parent_->children_||element.second.get().parent_->children().empty()||parent_filter.contains(element.second.get().parent_)) continue;
      parent_filter.emplace(element.second.get().parent_,true);
      auto children_vec = element.second.get().parent_->children();
      for(int i = 0;i <= element.second.get().parent_->children().size(); ++i)
      {
        if(additional_data_<0 &&  (i-additional_data_)%selector.factor_ == 0 && i>0 && inputs_.contains(&children_vec[i-1].get()))
        {
          append_result(&children_vec[i-1].get(),children_vec[i-1].get());
        }else if(additional_data_>=0 && children_vec.size() >=selector.factor_*i + additional_data_){
          int result_index = static_cast<int>(selector.factor_)*i + additional_data_-1;
          if(result_index >=0 && result_index < children_vec.size())
            append_result(&children_vec[result_index].get(),children_vec[result_index]);
        }
      }
    }
    return selector_visitor_result{std::move(result_), false};
  }
  selector_visitor::result_type selector_visitor::operator()(const ast::nth_child_odd_even &selector) {
    std::unordered_map<html::html_element*, bool> parent_filter;
    for(auto &element: inputs_){
      if(!is_valid(element.second.get())||!element.second.get().parent_||!element.second.get().parent_->children_||element.second.get().parent_->children().empty()||parent_filter.contains(element.second.get().parent_)) continue;
      parent_filter.emplace(element.second.get().parent_,true);
      unsigned int index = 1;
      for(auto &child :element.second.get().parent_->children())
      {
        if(index%2 == selector.odd_ && inputs_.contains(&child.get()))
          append_result(&child.get(),child);
        index++;
      }
    }
    return selector_visitor_result{std::move(result_), false};
  }
  selector_visitor::result_type selector_visitor::operator()(const ast::nth_child_constant &selector) {
    std::unordered_map<html::html_element*, bool> parent_filter;
    for(auto &element: inputs_){
      if(!is_valid(element.second.get())||!element.second.get().parent_||!element.second.get().parent_->children_||element.second.get().parent_->children().empty()||parent_filter.contains(element.second.get().parent_)) continue;
      parent_filter.emplace(element.second.get().parent_,true);
      unsigned int index = 1;
      for(auto &child :element.second.get().parent_->children())
      {
        if(index == selector.nth_ && inputs_.contains(&child.get()))
        {
          append_result(&child.get(),child);
          break;
        }
        index++;
      }
    }
    return selector_visitor_result{std::move(result_), false};
  }

  /// NthLastChild Selectors
  selector_visitor::result_type selector_visitor::operator()(const ast::last_nth_child_odd_even &selector) {
    std::unordered_map<html::html_element*, bool> parent_filter;
    for(auto &element: inputs_){
      if(!is_valid(element.second.get())||!element.second.get().parent_||!element.second.get().parent_->children_||element.second.get().parent_->children().empty()||parent_filter.contains(element.second.get().parent_)) continue;
      parent_filter.emplace(element.second.get().parent_,true);
      unsigned int index = 1;
      for(auto &child : std::ranges::reverse_view(element.second.get().parent_->children()))
      {
        if(index%2 == selector.odd_ && inputs_.contains(&child.get()))
          append_result(&child.get(),child);
        index++;
      }
    }
    return selector_visitor_result{std::move(result_), false};
  }
  selector_visitor::result_type selector_visitor::operator()(const ast::last_nth_child_constant &selector) {
    std::unordered_map<html::html_element*, bool> parent_filter;
    for(auto &element: inputs_){
      if(!is_valid(element.second.get())||!element.second.get().parent_||!element.second.get().parent_->children_||element.second.get().parent_->children().empty() ||parent_filter.contains(element.second.get().parent_)) continue;
      parent_filter.emplace(element.second.get().parent_,true);
      unsigned int index = 1;
      for(auto &child :std::ranges::reverse_view(element.second.get().parent_->children()))
      {
        if(index == selector.nth_ && inputs_.contains(&child.get()))
        {
          append_result(&child.get(),child);
          break;
        }
        index++;
      }
    }
    return selector_visitor_result{std::move(result_), false};
  }
  selector_visitor::result_type selector_visitor::operator()(const ast::last_nth_child_variant &selector) {
    int additional_data_ = 0;
    if(selector.optional_.has_value())
    {
      additional_data_ = selector.optional_->operator_=='+' ? static_cast<int>(selector.optional_->additional_data_):static_cast<int>(selector.optional_->additional_data_)*-1;
    }
    // variant to prevent match same parent many times
    std::unordered_map<html::html_element*, bool> parent_filter;
    for(auto &element: inputs_){
      if(!is_valid(element.second.get())||!element.second.get().parent_||!element.second.get().children_||element.second.get().parent_->children().empty()||parent_filter.contains(element.second.get().parent_)) continue;
      parent_filter.emplace(element.second.get().parent_,true);
      auto children_vec = element.second.get().parent_->children();
      for(int i = 0;i <= element.second.get().parent_->children().size(); ++i)
      {
        if(additional_data_<0 &&  (i-additional_data_)%selector.factor_ == 0 && i>0 && inputs_.contains(&children_vec[children_vec.size()-i].get()))
        {
          append_result(&children_vec[children_vec.size()-i].get(),children_vec[children_vec.size()-i].get());
        }else if(additional_data_>=0 && children_vec.size() >=selector.factor_*i + additional_data_){
          int result_index = children_vec.size()-(selector.factor_*i + additional_data_);
          if(result_index >=0 && result_index < children_vec.size())
            append_result(&children_vec[result_index].get(),children_vec[result_index]);
        }
      }
    }
    return selector_visitor_result{std::move(result_), false};
  }

  /*********************Operator Selectors*********************/
  selector_visitor_result selector_visitor::operator()(const ast::selector_operator &selector) {
    // Descendants Operator
    if(selector.operator_ == ' ')
    {
      for(auto &element: inputs_){
        if(!is_valid(element.second)) continue;
        element.second.get().descendants(result_);
      }
    }
      // Children Operator
    else if(selector.operator_ == '>'){
      for(auto &element: inputs_){
        // Skip nodes without children
        if(!is_valid(element.second) || !element.second.get().children_ || element.second.get().children_->empty()) continue;
        for(auto &child : *element.second.get().children_)
          append_result(&child.get(), child);
      }
    }
      // Sibling ~ Operator
    else if(selector.operator_ == '~'){
      for(auto &element: inputs_){
        // Skip nodes without parent
        if(!is_valid(element.second) || !element.second.get().parent_ || !element.second.get().parent_->children_ || element.second.get().parent_->children_->size()<=1) continue;
        bool found = false;
        for(auto &child: *element.second.get().parent_->children_){
          if(!is_valid(child)) continue;
          if(found) append_result(&child.get(), child);
          if(&child.get() == &element.second.get())
            found = true;
        }
      }
    }
      // Sibling + Operator
    else if(selector.operator_ == '+'){
      for(auto &element: inputs_){
        // Skip nodes without parent
        if(!is_valid(element.second) || !element.second.get().parent_ || !element.second.get().parent_->children_ || element.second.get().parent_->children_->size()<=1) continue;
        bool found = false;
        for(auto &child: *element.second.get().parent_->children_){

          if(found) {
            append_result(&child.get(), child);
            break;
          };
          if(&child.get() == &element.second.get())
            found = true;
        }
      }
    }
      // Comma Operator
    else if(selector.operator_ == ','){
      return selector_visitor_result{std::move(inputs_), true};
    }
    return selector_visitor_result{std::move(result_), false};
  }

  bool selector_visitor::is_valid(html::html_element &element) {
    return
      // InnerText,Footer and Comment shouldn't join the selector match
        element.type_ == html::html_element::ET_NORMAL || element.type_ == html::html_element::ET_SCRIPT;
        ;
  }
  void selector_visitor::append_result(html::html_element *pointer, std::reference_wrapper<html::html_element> element){
    if(!result_.contains(pointer)) result_.emplace(pointer, element);
  }

  selector_visitor::result_type selector_visitor::operator()(const ast::last_child &selector) {
    for(auto &element: inputs_){
      if(!is_valid(element.second.get())||!element.second.get().parent_||!element.second.get().parent_->children_||element.second.get().parent_->children().empty()) continue;
      auto children_vec = element.second.get().parent_->children();
      if((&children_vec.back().get()) == element.first)
      {
        append_result(element.first,element.second);
      }
    }
    return selector_visitor_result{std::move(result_), false};
  }

  selector_visitor::result_type selector_visitor::operator()(const ast::first_of_type &selector) {
    std::unordered_map<html::html_element*, bool> parent_filter;
    for(auto &element: inputs_){
      if(!is_valid(element.second.get())||!element.second.get().parent_||!element.second.get().parent_->children_||element.second.get().parent_->children().empty() ||parent_filter.contains(element.second.get().parent_)) continue;
      parent_filter.emplace(element.second.get().parent_,true);
      auto children_vec = element.second.get().parent_->children();
      std::unordered_map<std::string, std::pair<html::html_element*,std::reference_wrapper<html::html_element>>> type_map;
      for(auto &child :children_vec)
      {
        std::string tag_name = child.get().type_==html::html_element::ET_SCRIPT? "script":child.get().content();
        if(type_map.contains(tag_name) || !inputs_.contains(&child.get())) continue;
        type_map.emplace(tag_name,std::make_pair(&child.get(),child));
      }

      for(auto &result: type_map){
        append_result(result.second.first, result.second.second);
      }
    }
    return selector_visitor_result{std::move(result_), false};
  }

  selector_visitor::result_type selector_visitor::operator()(const ast::last_of_type &selector) {
    std::unordered_map<html::html_element*, bool> parent_filter;
    for(auto &element: inputs_){
      if(!is_valid(element.second.get())||!element.second.get().parent_||!element.second.get().parent_->children_||element.second.get().parent_->children().empty() ||parent_filter.contains(element.second.get().parent_)) continue;
      parent_filter.emplace(element.second.get().parent_,true);
      auto children_vec = element.second.get().parent_->children();
      std::unordered_map<std::string, std::pair<html::html_element*,std::reference_wrapper<html::html_element>>> type_map;
      for(auto &child : std::ranges::reverse_view(children_vec))
      {
        std::string tag_name = child.get().type_==html::html_element::ET_SCRIPT? "script":child.get().content();
        if(type_map.contains(tag_name) || !inputs_.contains(&child.get())) continue;
        type_map.emplace(tag_name,std::make_pair(&child.get(),child));
      }

      for(auto &result: type_map){
        append_result(result.second.first, result.second.second);
      }
    }
    return selector_visitor_result{std::move(result_), false};
  }

  selector_visitor::result_type selector_visitor::operator()(const ast::only_of_type &selector) {
    std::unordered_map<html::html_element*, bool> parent_filter;
    for(auto &element: inputs_){
      if(!is_valid(element.second.get())||!element.second.get().parent_||!element.second.get().parent_->children_||element.second.get().parent_->children().empty() ||parent_filter.contains(element.second.get().parent_)) continue;
      parent_filter.emplace(element.second.get().parent_,true);
      auto children_vec = element.second.get().parent_->children();
      std::unordered_map<std::string, std::pair<html::html_element*,std::reference_wrapper<html::html_element>>> type_map;
      std::unordered_map<std::string, bool> black_list;
      for(auto &child : children_vec)
      {
        std::string tag_name = child.get().type_==html::html_element::ET_SCRIPT? "script":child.get().content();
        if(black_list.contains(tag_name) || !inputs_.contains(&child.get())) continue;
        if(type_map.contains(tag_name))
        {
          type_map.erase(tag_name);
          black_list.emplace(tag_name, true);
          continue;
        }
        type_map.emplace(tag_name,std::make_pair(&child.get(),child));
      }

      for(auto &result: type_map){
        append_result(result.second.first, result.second.second);
      }
    }
    return selector_visitor_result{std::move(result_), false};
  }

  selector_visitor::result_type selector_visitor::operator()(const ast::empty_type &selector) {
    for(auto &element: inputs_){
      if(!is_valid(element.second) || !element.second.get().children().empty()) continue;
      append_result(element.first, element.second);
    }
    return selector_visitor_result{std::move(result_), false};
  }

  selector_visitor::result_type selector_visitor::operator()(const ast::enabled &selector) {
    for(auto &element: inputs_){
      if(!is_valid(element.second) || !element.second.get().can_be_disabled()) continue;
      if(element.second.get().disabled() == !selector.enabled_)
        append_result(element.first, element.second);
    }
    return selector_visitor_result{std::move(result_), false};
  }

  selector_visitor::result_type selector_visitor::operator()(const ast::checked &selector) {
    for(auto &element: inputs_){
      if(element.second.get().checked())
        append_result(element.first, element.second);
    }
    return selector_visitor_result{std::move(result_), false};
  }

  selector_visitor::result_type selector_visitor::operator()(const ast::not_ &selector) {

    return css3::selector_visitor::result_type();
  }


}