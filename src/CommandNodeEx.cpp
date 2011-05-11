
#include "inc.h"
#include "CommandNode.h"

namespace serecs { namespace Nodes{


	/**
	 *	Parse a text node, ie, do nothing!
	 *	@see Node::parse()
	 */
	void TextNode::parse(){}

	/**
	 *	Parse a generic (single expression) Node like "if (expression)"
	 *	@param theRegex the regular expression to validate the Node
	 *	@param error the error to report if the Node is invalid
	 *	@throws InvalidNodeExpression
	 *	@see Node::parse()
	 */
	void NodeSingleCondition::parse(const regex &theRegex,const char* error){
		cmatch res;
		int offset = this->textFragment->startIndex;
		if (regex_search(textFragment->rawTextFragment->c_str(),res,theRegex)) {
			CommandNode::generalParse(res[2].str(),offset,false);
			*this->textFragment->rawTextFragment = res[2].str();
			this->textFragment->startIndex += res[1].length();
			this->textFragment->endIndex = this->textFragment->startIndex + res[2].length();
		} else throwGeneric(error,offset);
	}

	/**
	 *	Parse a for Node
	 *	@throws InvalidNodeExpression
	 *	@see Node::parse()
	 */
	void NodeFor::parse(){
		static const regex regexForStart("(^\\s*for\\s*\\()(.*)\\)\\s*$");
		cmatch res;
		int offset = this->textFragment->startIndex;
		if (regex_search(textFragment->rawTextFragment->c_str(),res,regexForStart)) {
			CommandNode::generalParse(res[2].str(),offset,true);
		} else throwGeneric("Invalid 'for' expression.  Expected: 'for (expression;expression;expression)'",offset);
	}

	/**
	 *	Parse a while Node
	 *	@throws InvalidNodeExpression
	 *	@see Node::parse()
	 */
	void NodeWhile::parse(){
		static const regex regexWhileStart("(^\\s*while\\s*\\()(.*)\\)\\s*$");
		NodeSingleCondition::parse(regexWhileStart,"Invalid 'while' expression.  Expected: 'while (expression)'");
	}

	/**
	 *	Parse an if Node
	 *	@throws InvalidNodeExpression
	 *	@see Node::parse()
	 */
	void NodeIf::parse(){
		static const regex regexWhileStart("(^\\s*if\\s*\\()(.*)\\)\\s*$");
		NodeSingleCondition::parse(regexWhileStart,"Invalid 'if' expression.  Expected: 'if (expression)'");
	}

	/**
	 *	Parse an elsif Node
	 *	@throws InvalidNodeExpression
	 *	@see Node::parse()
	 */
	void NodeElsif::parse(){
		static const regex regexWhileStart("(^\\s*elsif\\s*\\()(.*)\\)\\s*$");
		NodeSingleCondition::parse(regexWhileStart,"Invalid 'elsif' expression.  Expected: 'elsif (expression)'");
	}

	/**
	 *	Parse an dowhile Node
	 *	@throws InvalidNodeExpression
	 *	@see Node::parse()
	 */
	void NodeDoWhile::parse(){
		static const regex regexWhileStart("(^\\s*dowhile\\s*\\()(.*)\\)\\s*$");
		NodeSingleCondition::parse(regexWhileStart,"Invalid 'dowhile' expression.  Expected: 'dowhile (expression)'");
	}

	/**
	 *	ErrorNode Constructor - create a node that will throw an error 
	 *		when parsing.  This is because Node::createNode should
	 *		always return a valid Node Object
	 *	@param tf the text fragment for this Node
	 *	@param errorText the text of the errror
	 *	@param startIndex the index where the text fragment occurs
	 */
	ErrorNode::ErrorNode(shared_ptr<TextFragment> theTf,string errorText,int theStartIndex) : Node(theTf,nodeError){
		this->errorText = errorText;
		this->startIndex = theStartIndex;
	}

	/**
	 *	Parse and error node.  Always throws an exception
	 *	@throws InvalidNodeExpression, always
	 *	@see Node::parse()
	 */
	void ErrorNode::parse(){
		throwGeneric(this->errorText,this->startIndex);
	}


	/**
	 *	Parse a switch Node
	 *	@throws InvalidNodeExpression
	 *	@see Node::parse()
	 */
	/*void NodeSwitch::parse(){
		static const regex regexWhileStart("^\\s*switch\\s*\\()(.*)\\$");
		NodeSingleCondition::parse(regexWhileStart,"Invalid 'switch' expression.  Expected: 'switch (expression)'");
	}*/


}	}	//end namespaces