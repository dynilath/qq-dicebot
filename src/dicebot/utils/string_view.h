#pragma once
#include <iterator>
#include <list>
#include <regex>
#include <string>
#include <type_traits>

#ifdef _DEBUG
#include <cassert>
#endif

namespace dicebot::utils {

    // std::string_view is working awfully with std::regex.
    // this is a replacement that support conversion from
    // sub_match and can convert to std::string
    template <typename _str_iter,
              typename std::enable_if_t<
                  std::is_base_of_v<typename std::bidirectional_iterator_tag,
                                    typename _str_iter::iterator_category>,
                  int> = 0>
    class basic_string_view : std::pair<_str_iter, _str_iter> {
        using iter_t = _str_iter;
        using c_t = typename iter_t::value_type;
        using trait_t = typename std::char_traits<c_t>::char_type;
        using riter_t = typename std::reverse_iterator<iter_t>;
        using sub_match_t = typename std::match_results<iter_t>::value_type;

    public:
        static_assert(std::is_same_v<c_t, trait_t>,
                      "value_type and trait_type require to be same.");

        using traits_type = typename iter_t::value_type;
        using value_type = traits_type;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using reference = value_type&;
        using const_reference = const value_type&;
        using iterator = iter_t;
        using const_iterator = iter_t;
        using reverse_iterator = riter_t;
        using const_reverse_iterator = riter_t;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;

    private:
        inline size_type _size() {
            auto diff = this->second - this->first;
            if (diff < 0)
                return 0;
            else
                return diff;
        }

    public:
        basic_string_view() noexcept : std::pair<iter_t, iter_t>() {}

        basic_string_view(iter_t first, iter_t second) noexcept
            : std::pair<iter_t, iter_t>(first, second) {}

        basic_string_view(const sub_match_t& sub_match) noexcept
            : std::pair<iter_t, iter_t>(sub_match.first, sub_match.second) {}

        basic_string_view(sub_match_t&& sub_match) noexcept
            : std::pair<iter_t, iter_t>(std::move(sub_match)) {}

        iter_t begin() const { return this->first; }

        iter_t end() const { return this->second; }

        iter_t cbegin() const { return this->first; }

        iter_t cend() const { return this->second; }

        riter_t rbegin() const {
            return std::make_reverse_iterator(this->second);
        }

        riter_t rend() const { return std::make_reverse_iterator(this->first); }

        riter_t crbegin() const {
            return std::make_reverse_iterator(this->second);
        }

        riter_t crend() const {
            return std::make_reverse_iterator(this->first);
        }

        size_type size() const {
            return std::distance(this->first, this->second);
        }

        template <std::enable_if_t<
                      std::is_same_v<typename std::random_access_iterator_tag,
                                     typename _str_iter::iterator_category>,
                      int> = 0>
        value_type operator[](size_type _Off) {
#ifdef _DEBUG
            assert(_Off < this->size());
#endif
            return this->first[_Off];
        }

        value_type operator[](size_type _Off) {
#ifdef _DEBUG
            assert(_Off < this->size());
#endif
            return *(this->first + _Off);
        }

        using string_t = ::std::basic_string<value_type>;
        operator string_t() const {
            return string_t(this->first, this->second);
        }
    };

    using string_view = basic_string_view<std::string::const_iterator>;
} // namespace dicebot::utils