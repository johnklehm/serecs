
#pragma once

#include "inc.h"
#include "PluginNode.h"


using namespace std::tr1;

namespace serecs{	namespace Nodes{

	
	/**
	 *	Parse the plugin node
	 *	@see Node::parse()
	 */
	void PluginNode::parse(){
		static regex name("^\\s*([A-Za-z_][A-Za-z_0-9]+)\\s*");
		static regex argument("\\s*(?:([A-Za-z_][A-Za-z_0-9]*)\\s*=\\s*|'((?:[^\\n\\r\\\\']|\\\\[^\\n\\r])+)'|\"((?:[^\\n\\r\\\\\"]|\\\\[^\\n\\r])+)\"|([0-9]+)|(\\$[A-Za-z_0-9]+))\\s*");
		static regex value("'((?:[^\\n\\r\\\\']|\\\\[^\\n\\r])+)'|\"((?:[^\\n\\r\\\\\"]|\\\\[^\\n\\r])+)\"|([0-9]+)|(\\$[A-Za-z_0-9]+)");

		cmatch res;
		if (regex_search(textFragment->rawTextFragment->c_str(),res,name)){
			namedArguments.reset(new std::map<std::string,std::string>);
			orderedArguments.reset(new std::map<std::string,std::string>);
			pluginName = res[1].str();

			std::string currentString = textFragment->rawTextFragment->substr(res.length());
			std::string name,value;
			int curIndex = res.length();
			int startIndex = 0;
			int endIndex = 0;
				
			while ( !currentString.empty() ){
				cout<<currentString<<endl;
				if (regex_search(currentString.c_str(),res,argument)){

					if (res[1].matched){			//name-value pair



						if (res[1].matched){		//string single quotes

						} else if (res[2].matched){	//string double quotes
						
						} else if (res[3].matched){		//number

						} else if (res[4].matched){		//variable

						}

					} else if (res[2].matched){		//string single quotes

					} else if (res[3].matched){		//string double quotes

					} else if (res[4].matched){		//number

					} else if (res[5].matched){		//variable

					}

					currentString = currentString.substr(res.length());
					cout<<"match"<<endl;

				} else throwGeneric("Unrecognized character sequence",curIndex);
			}				

		} else throwGeneric("Invalid Plugin Name",textFragment->startIndex);

	}

	void PluginNode::verify(){

	}

	void PluginNode::compile(){
		//'((?:[^\n\r\\^']|\\[^\n\r])+)'|([0-9]+)|(true)|(false)
		// \s*([A-Za-z_][A-Za-z_0-9]*)\s*=\s*
	}


}	}