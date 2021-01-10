#pragma once
#include <vector>
#include <cstdint>
#include <iterator>

namespace dicebot::utils {

namespace impl{
void int2utf8byte(std::vector<unsigned char>& cont, unsigned int value){
    if(value <= 0x7f){
        cont.push_back(value);
    }
    else if(value <= 0x07ff){
        unsigned char high = 0b11000000 | ( value >> 6);
        unsigned char low = 0b10000000 | ( value & 0b00111111);
        cont.push_back(high);
        cont.push_back(low);
    }
    else if(value <= 0xffff){
        unsigned char high = 0b11100000 | (value >> 12);
        unsigned char mid = 0b10000000 | ((value >> 6) & 0b00111111);
        unsigned char low = 0b10000000 | (value & 0b00111111);
        cont.push_back(high);
        cont.push_back(mid);
        cont.push_back(low);
    }
    else if(value <= 0x10ffff){
        unsigned char high = 0b11110000 | (value >> 18);
        unsigned char mid1 = 0b10000000 | ((value >> 12) & 0b00111111);
        unsigned char mid2 = 0b10000000 | ((value >> 6) & 0b00111111);
        unsigned char low = 0b10000000 | (value & 0b00111111);
        cont.push_back(high);
        cont.push_back(mid1);
        cont.push_back(mid2);
        cont.push_back(low);
    }
}

template<typename Iter>
Iter utf8byte2int(Iter begin, Iter end, unsigned int& value){
#define BAD_RETURN \
    {value = (unsigned int)-1; \
    return end;}
    unsigned char lead = *begin;
    if(lead <= 0x7f){
        value = lead;
        return begin+1;
    }
    else if(lead >= 0b11111000) BAD_RETURN
    else if(lead >= 0b11110000){
        unsigned char high = *begin;
        begin++;
        if(begin == end) BAD_RETURN
        unsigned char mid1 = *begin;
        begin++;
        if(begin == end) BAD_RETURN
        unsigned char mid2 = *begin;
        begin++;
        if(begin == end) BAD_RETURN
        unsigned char low = *begin;
        
        value = high & 0b11111;
        value = (value << 6) | (mid1 & 0b111111);
        value = (value << 6) | (mid2 & 0b111111);
        value = (value << 6) | (low & 0b111111);
        return begin+1;
    }
    else if(lead >= 0b11100000){
        unsigned char high = *begin;
        begin++;
        if(begin == end) BAD_RETURN
        unsigned char mid = *begin;
        begin++;
        if(begin == end) BAD_RETURN
        unsigned char low = *begin;

        value = high & 0b11111;
        value = (value << 6) | (mid & 0b111111);
        value = (value << 6) | (low & 0b111111);
        return begin+1;
    }
    else if(lead >= 0b11000000){
        unsigned char high = *begin;
        begin++;
        if(begin == end) BAD_RETURN
        unsigned char low = *begin;
        
        value = high & 0b11111;
        value = (value << 6) | (low & 0b111111);
        return begin+1;
    }
    BAD_RETURN
#undef BAD_RETURN
}
}

template<typename Iter>
std::vector<unsigned char> integers_2_utf8_bytes(Iter begin, Iter end){
    std::vector<unsigned char> ret;
    while(begin != end){
        impl::int2utf8byte(ret, *begin);
        begin++;
    }
    return ret;
}

template<typename Iter>
std::vector<unsigned int> utf8_bytes_2_integers(Iter begin, Iter end){
    std::vector<unsigned int> ret;
    while(begin != end){
        decltype(ret)::value_type value;
        begin = impl::utf8byte2int(begin,end,value);
        if(value != (unsigned int)-1) ret.push_back(value);
    }
    return ret;
}

}
