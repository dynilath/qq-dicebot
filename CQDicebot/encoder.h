#pragma once
class encoder
{
	std::vector<char> * vec_container;
public:
	encoder();
	~encoder();
	void addInt(int input);
	int getInt();
	void setChar(const char * source, size_t size);
	char * getData();
	uint32_t getSize();
	char * getBase64();
	void setBase64(const char * source, size_t size);
};

