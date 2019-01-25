#include "./dice_utils.h"

using namespace dicebot;

void utils::split_line(std::string const & source, std::list<std::string>& source_splits){
    if(source.length() > 2){
        unsigned int iter_source = 0;
        std::string spliter = "\r\n";
        while(iter_source < source.length()){
            unsigned int i_target = source.find(spliter, iter_source);
            if(i_target == std::string::npos){
                if(i_target > iter_source) source_splits.push_back(source.substr(iter_source, source.length() - iter_source));
                break;
            }
            else{
                if(i_target > iter_source) source_splits.push_back(source.substr(iter_source, i_target - iter_source));
                iter_source = i_target + spliter.length();
            }
        }
    }
    else if(source.length() > 0){
        source_splits.push_back(source);
    }
}

void utils::remove_space_and_tab(std::string & source){
    //i cant erase(iter,str.end()), so switch to traditional method
    unsigned int iter = 0;
    unsigned int i_continuous_space_count = 0;
    unsigned int i_erase_start = 0;
    while(iter < source.length()){
        if(source[iter] == ' ' || source[iter] == '\t'){
            if(iter + 1 != source.length()){
                if(i_continuous_space_count == 0) i_erase_start = iter;
                i_continuous_space_count++;
                iter++;
            }
            else{
                source.erase(i_erase_start, source.length() - i_erase_start);
                iter++;
            }
        }
        else if(i_continuous_space_count > 0){
            source.erase(i_erase_start, i_continuous_space_count);
            iter = ++i_erase_start;
            i_continuous_space_count = 0;
        }
        else{
            i_erase_start = ++iter;
        }
    }
}

void utils::quick_sort(int * origin, int * pilot, int first, int last){
    if(last <= first)
        return;

    int pivot = origin[first];
    int pivot_pilot = pilot[first];
    int left = first + 1;
    int right = last;
    int temp;
    while(left <= right){
        while(origin[right] > pivot&&right >= left)
            right--;

        while(origin[left] < pivot&&left <= right)
            left++;

        if(left >= right)
            break;

        temp = origin[left];
        origin[left] = origin[right];
        origin[right] = temp;
        temp = pilot[left];
        pilot[left] = pilot[right];
        pilot[right] = temp;
        left++, right--;
    }
    origin[first] = origin[right];
    origin[right] = pivot;

    pilot[first] = pilot[right];
    pilot[right] = pivot_pilot;

    dicebot::utils::quick_sort(origin, pilot, first, right - 1);
    dicebot::utils::quick_sort(origin, pilot, left, last);
}

bool utils::find_point(std::string const & source, size_t & content_begin){
    size_t point = 0;
    for(;;point ++){
        if(point == source.size()) return false;
        else if(source[point] == ' ') continue;
        else if(source[point] == '.') break;
        else return false;
    }
    content_begin = point + 1;
    for(;content_begin < source.size();content_begin ++){
        if(source[content_begin] == ' ') continue;
        else return true;
    }
    return false;
}

bool utils::find_space(std::string const & source, size_t & pos){
    pos = 0;
    do{
        if(pos == source.length()) return false;
        else if(source[pos] == ' ') return true;
    }while(++pos);
    return false;
}