
#pragma once

#include "inc.h"

namespace serecs {	namespace templates {

	class Fragment;
	typedef std::tr1::shared_ptr<Fragment> FragmentPointer;

	class Fragment {
	public:
		int start, end;
		int type;
		int numExpressions;

		Fragment(int tStart,int tEnd){
			this->start = tStart;
			this->end = tEnd;
			this->type = TEXT;
			this->numExpressions = 0;
		}

		Fragment(int tStart,int tEnd,int tType,int tNE){
			this->start = tStart;
			this->end = tEnd;
			this->type = tType;
			this->numExpressions = tNE;
		}

		static FragmentPointer make(int tStart,int tEnd,int tType,int tNE){
			return FragmentPointer( new Fragment(tStart,tEnd,tType,tNE) );
		}

		static FragmentPointer makeText(int tStart, int tEnd){
			return FragmentPointer( new Fragment(tStart,tEnd) );
		}		

		static const int INVALID	=-1;
		static const int UNKNOWN	= 0;
		static const int TEXT		= 1;
		static const int BLOCK		= 2;
		static const int OUTPUT		= 3;
		static const int CODE		= 4;
		static const int COMMENT	= 5;
		static const int PLUGIN		= 6;
		

		/*
		 symControlBlock,
			symOutput,
			symRawCode,
			symComment,
			symPlugin;
			*/

	};

	typedef std::list<FragmentPointer> ListOfFragmentPointers;
	typedef std::tr1::shared_ptr<ListOfFragmentPointers> ListOfFragmentPointersPointer;

}	}		//end namespace serecs::templates::
