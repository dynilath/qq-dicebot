#include "stdafx.h"
#include "utility.h"

void splitLine(std::string & source, std::vector<std::string>& source_splits)
{
	if (source.length()>2) {
		unsigned int iter_source = 0;
		std::string spliter = "\r\n";
		while (iter_source < source.length()) {
			unsigned int i_target = source.find(spliter, iter_source);
			if (i_target == std::string::npos) {
				if (i_target > iter_source) source_splits.push_back(source.substr(iter_source, source.length() - iter_source));
				break;
			}
			else {
				if (i_target > iter_source) source_splits.push_back(source.substr(iter_source, i_target - iter_source));
				iter_source = i_target + spliter.length();
			}
		}
	}
}

void removeSpaceAndTab(std::string & source)
{
	//i cant erase(iter,str.end()), so switch to traditional method
	unsigned int iter = 0;
	unsigned int i_continuous_space_count = 0;
	unsigned int i_erase_start = 0;
	while (iter < source.length()) {
		if (source[iter] == ' ' || source[iter] == '\t') {
			if (iter + 1 != source.length()) {
				if (i_continuous_space_count == 0) i_erase_start = iter;
				i_continuous_space_count++;
				iter++;
			}
			else {
				source.erase(i_erase_start, source.length() - i_erase_start);
				iter++;
			}
		}
		else if (i_continuous_space_count > 0) {
			source.erase(i_erase_start, i_continuous_space_count);
			iter = ++i_erase_start;
			i_continuous_space_count = 0;
		}
		else {
			i_erase_start = ++iter;
		}
	}
}

void quickSort(int * origin, int * pilot, int first, int last) {
	if (last <= first)
		return;

	int pivot = origin[first];
	int pivot_pilot = pilot[first];
	int left = first + 1;
	int right = last;
	int temp;
	while (left <= right) {
		while (origin[right] > pivot&&right >= left)
			right--;

		while (origin[left] < pivot&&left <= right)
			left++;

		if (left >= right)
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

	quickSort(origin, pilot, first, right - 1);
	quickSort(origin, pilot, left, last);
}
