#pragma once

#include <vector>
#include "../cqsdk/utils/base64.h"

namespace base64 = cq::utils::base64;

namespace dicebot{
	class encoder
	{
		std::vector<char> * vec_container;
	public:
		encoder(){
			vec_container = new std::vector<char>();
		}

		~encoder(){ if(vec_container != nullptr) delete vec_container; }

		void addInt(int input){
			char * pchar_source = (char *)(&input);
			for (int i_iter = 0; i_iter < sizeof(int); i_iter++) {
				vec_container->push_back(pchar_source[i_iter]);
			}
		}

		int getInt(){
			int i_ret = -1;
			char * pchar_source = (char *)(&i_ret);
			for (int i_iter = 0; i_iter < sizeof(int); i_iter++) {
				pchar_source[i_iter] = vec_container->at(i_iter);
			}
			vec_container->erase(vec_container->begin(),vec_container->begin() + sizeof(int));
			return i_ret;
		}

		void setChar(const char * source, size_t size){
			vec_container = new std::vector<char>();
			for (unsigned int i_iter = 0; i_iter < size; i_iter++) {
				vec_container->push_back(source[i_iter]);
			}
		}

		char * getData(){
			return vec_container->data();
		}

		uint32_t getSize(){
			return vec_container->size();
		}

		std::string getBase64(){
			std::string ret = base64::encode((const unsigned char *)(vec_container->data()), vec_container->size());
			return ret;
		}

		void setBase64(const std::string &source){
			std::string target = base64::decode(source);
			if (this->vec_container != nullptr) delete(this->vec_container);
			this->vec_container = new std::vector<char>();
			vec_container->resize(target.size());
			vec_container->assign(target.begin(),target.end());
		}

	};
}
