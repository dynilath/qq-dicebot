#pragma once

#include <sstream>
#include <type_traits>

namespace dicebot {
    class oarchive {
        using stream_t = std::ostream;
        stream_t& out_stream;
        template <class T>
        oarchive& archive_arithmetic(T val) {
            out_stream.write(reinterpret_cast<const char*>(&val), sizeof(val));
            return *this;
        }

    public:
        oarchive(stream_t& outs) : out_stream(outs) { out_stream.seekp(std::ios_base::beg); }

        template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
        oarchive& operator<<(T val) {
            return archive_arithmetic(val);
        }

        template <class T, std::enable_if_t<std::is_enum_v<T>, int> = 0>
        oarchive& operator<<(T val) {
            return archive_arithmetic(val);
        }

        oarchive& operator<<(std::string val) {
            archive_arithmetic(val.size());
            out_stream.write(reinterpret_cast<const char*>(&val[0]), val.size());
            return *this;
        }
    };

    class iarchive {
        using stream_t = std::istream;
        stream_t& in_stream;
        template <class T>
        iarchive& archive_arithmetic(T& val) {
            in_stream.read(reinterpret_cast<char*>(&val), sizeof(val));
            return *this;
        }

    public:
        iarchive(stream_t& ins) : in_stream(ins) { in_stream.seekg(std::ios_base::beg); }

        template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
        iarchive& operator>>(T& val) {
            return archive_arithmetic(val);
        }

        template <class T, std::enable_if_t<std::is_enum_v<T>, int> = 0>
        iarchive& operator>>(T& val) {
            return archive_arithmetic(val);
        }

        iarchive& operator>>(std::string& val) {
            size_t str_size;
            archive_arithmetic(str_size);
            val.resize(str_size);
            in_stream.read(reinterpret_cast<char*>(&val[0]), str_size);
            return *this;
        }
    };
}