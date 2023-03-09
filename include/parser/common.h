/***********************************************************************************************************************
 * @author Hackman Lo
 * @file common.h
 * @description 
 * @created_at 2022-12-28
***********************************************************************************************************************/

#ifndef ZK_CPP_SPIDER_COMMON_H
#define ZK_CPP_SPIDER_COMMON_H
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/utility/error_reporting.hpp>
namespace ankh::html::common {
    using namespace boost::spirit::x3;
    struct error_handler
    {
      template <typename Iterator, typename Exception, typename Context>
      error_handler_result on_error(
          Iterator& first, Iterator const& last
          , Exception const& x, Context const& context)
      {
        auto& error_handler = get<error_handler_tag>(context).get();
        std::string message = "Error! Expecting: " + x.which() + " here:";
        error_handler(x.where(), message);
        throw std::logic_error(message);
        return error_handler_result::fail;
      }

      template <typename Iterator, typename Exception, typename Context>
      void on_success(
          Iterator& first, Iterator const& last
          , Exception const& x, Context const& context)
      {
        //std::cout<< boost::spirit::x3::get<helpers::tag_name>(context)<< std::endl;
      }
    };
}

#endif //ZK_CPP_SPIDER_COMMON_H
