#ifndef _POINTERADDRESSUTILS_H_
#define _POINTERADDRESSUTILS_H_
#include <iostream>
#include <string>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include <ros/ros.h>

using namespace std;

template <typename T>
class PointerAddressUtils
{
	public:
	//PointerAddressUtils<T>{};
	//~PointerAddress{};

	std::string convertPointerToStringAddress(const T* obj)
	{
		intptr_t address(reinterpret_cast<intptr_t>(obj));
		std::stringstream ss;
		ss << address;
		return ss.str();
	}

	void convertAddressStringToPointer(const std::string& address, T** pointer)
	{
		std::stringstream ss;
		ss << address;
		int tmp(0);
		if(!(ss >> tmp)) ROS_ERROR("Failed to convert address string to pointer");
		*pointer = reinterpret_cast<T*>(tmp);
	}
};
#endif
