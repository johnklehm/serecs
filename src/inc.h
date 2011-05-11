
//#define USE_BOOST
#define BOOST_ALL_NO_LIB 

#define BOOST_FILESYSTEM_VERSION 3
#include <boost/filesystem.hpp>

#ifdef USE_BOOST
	
	#include <boost/tr1/memory.hpp>
	#include <boost/tr1/regex.hpp>
	using namespace std::tr1;
#elif defined(USE_TR1)
	#include <tr1/memory>
	#include <tr1/regex>
#else
	#include <memory>
	#include <regex>
#endif

#include <string>
#include <vector>
#include <map>
#include <list>
#include <iostream>
#include <conio.h>
#include <exception>
#include <stack>
#include <limits.h>
