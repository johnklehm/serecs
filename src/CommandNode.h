
#pragma once

#include "inc.h"
#include "TextFragment.h"
#include "Node.h"

namespace serecs{	namespace Nodes{


	class CommandNode : public Node{
	public:
		CommandNode(shared_ptr<TextFragment> theTf,NodeType theType) : Node(theTf,theType){}

	protected:
		static const shared_ptr<regex> regexStringSingleQuotes;
		static const shared_ptr<regex> regexStringDoubleQuotes;
		static const shared_ptr<regex> regexLineComment;
		static const shared_ptr<regex> regexMultilineComment;
		static const shared_ptr<regex> regexRegex;

		static void generalParse(string &text,int offset,bool isForNode);
	};

	class NodeFor:public CommandNode{
	public:
		NodeFor(shared_ptr<TextFragment> theTf) : CommandNode(theTf,nodeFor){}
		virtual void parse();
	};

	class NodeSingleCondition:public CommandNode{
	protected:
		NodeSingleCondition(shared_ptr<TextFragment> theTf,NodeType theType): CommandNode(theTf,theType){};
		void parse(const regex &theRegex, const char* error);
	};

	class NodeWhile:public NodeSingleCondition{
	public:
		NodeWhile(shared_ptr<TextFragment> theTf) : NodeSingleCondition(theTf,nodeWhile){}
		virtual void parse();
	};

	class NodeDoWhile:public NodeSingleCondition{
	public:
		NodeDoWhile(shared_ptr<TextFragment> theTf) : NodeSingleCondition(theTf,nodeDoWhile){}
		virtual void parse();
	};

	class NodeIf:public NodeSingleCondition{
	public:
		NodeIf(shared_ptr<TextFragment> theTf) : NodeSingleCondition(theTf,nodeIf){}
		virtual void parse();
	};

	class NodeElsif:public NodeSingleCondition{
	public:
		NodeElsif(shared_ptr<TextFragment> theTf) : NodeSingleCondition(theTf,nodeElsif){}
		virtual void parse();
	};

	class NodeEndIf:public CommandNode{
	public:
		NodeEndIf(shared_ptr<TextFragment> theTf) : CommandNode(theTf,nodeEndIf){}
		virtual void parse(){};
	};

	class NodeEndWhile:public CommandNode{
	public:
		NodeEndWhile(shared_ptr<TextFragment> theTf) : CommandNode(theTf,nodeEndWhile){}
		virtual void parse(){};
	};

	class NodeEndFor:public CommandNode{
	public:
		NodeEndFor(shared_ptr<TextFragment> theTf) : CommandNode(theTf,nodeEndFor){}
		virtual void parse(){};
	};

	/*
	nodeSwitch,nodeCase,
	nodeEndFor,nodeEndIf,nodeEndWhile,
	*/

		/*
	class NodeSwitch:NodeSingleCondition{
	public:
		NodeSwitch(shared_ptr<TextFragment> theTf) : NodeSingleCondition(theTf,nodeSwitch){}
		virtual void parse();
	};

	class NodeCase:Node{
	public:
		NodeCase(shared_ptr<TextFragment> theTf) : Node(theTf,nodeCase){}
		//virtual void parse(){generalParse(false);}
	};

	class NodeElse:Node{
	public:
		NodeElse(shared_ptr<TextFragment> theTf) : Node(theTf,nodeElse){}
		//virtual void parse(){}
	};
	*/
}	}//end namespaces
