#pragma once
#include <list>
#include <string>
#include <iterator>
#include <type_traits>
#include <regex>

namespace dicebot::utils {

    // std::string_view is working awfully with std::regex.
    // this is a replacement that support conversion from 
    // sub_match and can convert to std::string
    template<typename _string_iter_type, 
        typename std::enable_if_t<
            std::is_base_of_v<typename std::bidirectional_iterator_tag, typename _string_iter_type::iterator_category>,
            int> = 0>
    class basic_string_view : std::pair<_string_iter_type, _string_iter_type> {
        using iter_t = typename _string_iter_type;
        using c_t = typename iter_t::value_type;
        using trait_t = typename std::char_traits<c_t>::char_type;
        using riter_t = typename std::reverse_iterator<iter_t>;
        using sub_match_t = typename std::match_results<iter_t>::value_type;
    public:

        static_assert(std::is_same_v<typename c_t, typename trait_t>,"");

        using traits_type = typename iter_t::value_type;
        using value_type = typename traits_type;
        using pointer = typename value_type*;
        using const_pointer = typename const value_type*;
        using reference = typename value_type&;
        using const_reference = typename const value_type&;
        using iterator = typename iter_t;
        using const_iterator = typename iter_t;
        using reverse_iterator = typename riter_t;
        using const_reverse_iterator = typename riter_t;
        using size_type = typename std::size_t;
        using difference_type = typename std::ptrdiff_t;

        basic_string_view() noexcept
        : std::pair<iter_t, iter_t>()
        {}

        basic_string_view(iter_t first, iter_t second) noexcept
        : std::pair<iter_t, iter_t>(first, second)
        {}

        basic_string_view(const basic_string_view& other) noexcept 
        : std::pair<iter_t, iter_t>(other.first, other.second)
        {}

        basic_string_view(const sub_match_t& sub_match) 
        : std::pair<iter_t, iter_t>(sub_match.first, sub_match.second)
        {}
        
        iter_t begin() const{
            return this->first;
        }

        iter_t end() const{
            return this->second;
        }

        iter_t cbegin() const{
            return this->first;
        }

        iter_t cend() const{
            return this->second;
        }

        riter_t rbegin() const{
            return std::make_reverse_iterator(this->second);
        }

        riter_t rend() const{
            return std::make_reverse_iterator(this->first);
        }

        riter_t crbegin() const{
            return std::make_reverse_iterator(this->second);
        }

        riter_t crend() const{
            return std::make_reverse_iterator(this->first);
        }

        size_type size() const{
            return std::distance(this->first,this->second);
        }

        operator std::string() const{ return std::string(this->first, this->second); }
    };

    using string_view = basic_string_view<std::string::const_iterator>;

    std::list<utils::string_view> split_line(std::string const&);
    bool trim(utils::string_view &);
    bool jump_to_front_of_point(utils::string_view &);

} // namespace dicebot::utils
