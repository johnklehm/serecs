
#pragma once

#include "inc.h"

using namespace std;


class TextFragment{
public:
	TextFragment(){
		this->type = 0;
	}

	TextFragment(shared_ptr<string> TextFragment,char theType,int start,int end){
		this->rawTextFragment = TextFragment;
		this->type = theType;
		this->startIndex = start;
		this->endIndex = end;
	}


	int startIndex, endIndex;
	shared_ptr<std::string> rawTextFragment;
	
	char type;
};


typedef shared_ptr<vector<shared_ptr<TextFragment> > > SharedTextFragmentVector;

