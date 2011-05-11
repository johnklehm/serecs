
#pragma once

#include "inc.h"
#include "Node.h"
#include "TextFragment.h"

namespace serecs{	namespace Nodes{

	class PluginNode:Node{
	public:

		shared_ptr<std::map<std::string,std::string> > namedArguments;
		shared_ptr<std::map<std::string,std::string> > orderedArguments;
		std::string pluginName;

		/**
		 *	Constructor.  Sets type to nodePlugin
		 *	@param theTf text fragment associated with this node
		 */
		PluginNode(shared_ptr<TextFragment> theTf) : Node(theTf,nodePlugin){}

		virtual void parse();
		virtual void verify();
		virtual void compile();

		class Argument{
		public:
			std::string name;
			std::string value;
			int number;
			int type;

			Argument(std::string &tName,std::string &tValue,int tNum,int tType){
				this->name = tName;
				this->value = tValue;
				this->number = tNum;
				this->type = tType;
			}

			static const int typeTrue		= 2;
			static const int typeFalse		= 3;
			static const int typeString		= 4;
			static const int typeNumber		= 8;
			static const int typeVariable	= 16;
		};
	};

}	}