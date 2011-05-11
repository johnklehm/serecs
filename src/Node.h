
#pragma once

#include "inc.h"
#include "TextFragment.h"

namespace serecs{	namespace Nodes{

	using namespace std;

	void throwGeneric(string str,int start);

	enum NodeType {
		nodeText,
		nodeFor,nodeIf,nodeElse,nodeWhile,nodeDoWhile,/*nodeSwitch,nodeCase,nodeEndSwitch*/
		nodeEndFor,nodeEndIf,nodeEndWhile,
		nodeElsif,
		nodeUnknown,
		nodeOutput,
		nodeConstant,
		nodePlugin,
		nodeError
	};

	/**
	 *	An exception specific to an invalid node
	 */
	class InvalidNodeExpression : public std::exception  {
	public:
		/**
		 *	Constructor
		 *	@param theMessage the message to report
		 *	@param theIndex the startIndex of the message
		 */
		InvalidNodeExpression(string theMessage,int theIndex){
			this->message = theMessage;
			this->index = theIndex;
		}

		int index;
		string message;
	};

	class Node{
	public:
		Node(){}

		/**
		 *	Parse the node, making sure that it is a valid node
		 */
		virtual void parse(){}

		/**
		 *	Verify that the node is valid (ie a CommandNode is valid ecmascript)
		 */
		virtual void verify(){}

		/**
		 *	TODO
		 */
		virtual void compile(){}

		static shared_ptr<Node> createNode(shared_ptr<TextFragment> tf);

		NodeType type;	

	protected:
		Node(shared_ptr<TextFragment> theTf,NodeType theType){
			this->type = theType;
			this->textFragment = theTf;
		}

		shared_ptr<TextFragment> textFragment;
	
		std::vector< shared_ptr<Node> > children;
		weak_ptr<Node> parent;

		
	};

	class TextNode:public Node{
	public:
		TextNode(shared_ptr<TextFragment> theTf) : Node(theTf,nodeText){}
		virtual void parse();
	};

	class ErrorNode:public Node{
	public:
		ErrorNode(shared_ptr<TextFragment> theTf,string errorText,int theStartIndex);
		virtual void parse();
	private:
		std::string errorText;
		int startIndex;
	};

}	}//end namespaces
