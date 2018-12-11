#include <vector>
#include "encoder.h"
#include "base64.h"


encoder::encoder()
{
	vec_container = new std::vector<char>();
}


encoder::~encoder()
{
}

void encoder::addInt(int input)
{
	char * pchar_source = (char *)(&input);
	for (int i_iter = 0; i_iter < sizeof(int); i_iter++) {
		vec_container->push_back(pchar_source[i_iter]);
	}
}

int encoder::getInt()
{
	int i_ret = -1;
	char * pchar_source = (char *)(&i_ret);
	for (int i_iter = 0; i_iter < sizeof(int); i_iter++) {
		pchar_source[i_iter] = vec_container->at(i_iter);
	}
	vec_container->erase(vec_container->begin(),vec_container->begin() + sizeof(int));
	return i_ret;
}

void encoder::setChar(const char * source, size_t size)
{
	vec_container = new std::vector<char>();
	for (unsigned int i_iter = 0; i_iter < size; i_iter++) {
		vec_container->push_back(source[i_iter]);
	}
}

char * encoder::getData()
{
	return vec_container->data();
}

size_t encoder::getSize()
{
	return vec_container->size();
}

char * encoder::getBase64()
{
	int i_encode_len = Base64::EncodedLength(vec_container->size());
	char * out = new char[i_encode_len];
	Base64::Encode(vec_container->data(), vec_container->size(), out, i_encode_len);
	return out;
}

void encoder::setBase64(const char * source, size_t size)
{
	int i_decode_len = Base64::DecodedLength(source, size);
	if (this->vec_container != nullptr) delete(this->vec_container);
	this->vec_container = new std::vector<char>();
	vec_container->resize(i_decode_len);
	Base64::Decode(source, size, vec_container->data(), i_decode_len);
}
