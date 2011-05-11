
#pragma once


#include "../inc.h"
#include "../PluginNode.h"


class PreParserTest{
	
public:
	static void run(){

		using namespace serecs::Nodes;
		using namespace std::tr1;


		shared_ptr<std::string> text(new std::string("include file='dog.txt' cat='cat.txt'") );
		shared_ptr<TextFragment> tf(new TextFragment(text, '#', 0, 0 ) );

		PluginNode pn(tf);

		try {
			pn.parse();
			cout<<"no exception";
		} catch (InvalidNodeExpression &e){
			cout<<e.message<<endl;
		}



	}
};