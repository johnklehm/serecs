

#include "CommandNode.h"

using namespace std::tr1;

namespace serecs{ namespace Nodes{

	const shared_ptr<regex> CommandNode::regexStringSingleQuotes(new regex( "(?:[^'\\\\]+|(?:\\\\(?:[bfnrtv'\"\\\\]|[0-3]?[0-7]{1,2}|x[0-9a-f]{2}|u[0-9a-f]{4})))+'"));
	const shared_ptr<regex> CommandNode::regexStringDoubleQuotes(new regex("(?:[^\"\\\\]+|(?:\\\\(?:[bfnrtv'\"\\\\]|[0-3]?[0-7]{1,2}|x[0-9a-f]{2}|u[0-9a-f]{4})))+\""));
	const shared_ptr<regex> CommandNode::regexLineComment(new regex("[^\\n\\r]*\\r?\\n"));
	const shared_ptr<regex> CommandNode::regexMultilineComment(new regex(".*?\\*/"));
	const shared_ptr<regex> CommandNode::regexRegex(new regex("(?:[^/\\\\]|\\\\.)+/"));


	/**
	 *	Throw a generic exception
	 *	@throws InvalidNodeExpression
	 */
	void throwGeneric(string str,int start){
		throw InvalidNodeExpression(str,start);
	}	

	/**
	 *	Parse a condition/for construct to verify that it is correct
	 *	@param isForNode is this a for Node that we're parsing
	 *	@throws InvalidNodeExpression
	 */
	void CommandNode::generalParse(string &text,int offset,bool isForNode = false){

		cmatch res;
		stack<int> parens;

		int numExpressions = 0;
		int index = 0;

		int length = text.length();
		
		if (!isForNode)	numExpressions = 2;

		while (index < length){
			index = text.find_first_of(";()\"'/",index);
				
			if (index == string::npos)
				if (numExpressions == 2) break;
				else{
					string temp = "Invalid 'for' expression. Expecting 3 substatements, ";
					temp = temp + (numExpressions == 1 ? "2" : "1" ) + " found";
					throwGeneric(temp,offset + length);
				}
				
			switch(text.at(index)){
			case '(':
				parens.push(index);
				break;
			case ')':
				if (parens.empty())	throwGeneric("Unexpected closing parenthesis",offset + index);
				else parens.pop();
				break;
			case ';':
				if (!isForNode)	throwGeneric("Unexpected character ';'",index);
				if (!parens.empty()) throwGeneric("Unclosed parenthesis",offset + parens.top());
				else numExpressions++;
				break;
			case '"':
				if (regex_search(text.substr(index).c_str(),res,*CommandNode::regexStringDoubleQuotes)){
					index += res[0].length();
				} else throwGeneric("Unclosed string literal",offset + index);
				break;
			case '\'':
				if (regex_search(text.substr(index).c_str(),res,*CommandNode::regexStringSingleQuotes)){
					index += res[0].length();
				} else throwGeneric("Unclosed string literal",offset + index);
				break;

			case '/':
					
				if (index + 1 < length){
						
					char nextChar = text[index+1];
					if (nextChar == '/')throwGeneric("Single line comments are not allowed in template Nodes",offset+index);
					else if (nextChar == '*'){
						if (regex_search(text.substr(index+1).c_str(),res,*CommandNode::regexMultilineComment)){
							index += res[0].length() ;
						} else throwGeneric("Unclosed comment",offset+index);
					} else {	//must be regular expression
						if (regex_search(text.substr(index+1).c_str(),res,*CommandNode::regexRegex)){
							index += res[0].length() ;
						} else throwGeneric("Unexpected character '/' (did you forget to close a regular expression?)",offset+index);
					}
				} else throwGeneric("Unexpected character '/'",offset+index);

				break;
			}

			index++;
		}
	
	}

	shared_ptr<Node> Node::createNode(shared_ptr<TextFragment> tf){
		static const regex regexNodes("^\\s*(while)|(dowhile)|(for)|(if)|(elsif)|(elseif)|(switch)|(case)|(?:(?:(endif)|(endfor)|(endwhile)|(endswitch)|(endcase))\\s*$)");
		cmatch res;

		shared_ptr<Node> cmd;

		if (regex_search(tf->rawTextFragment->c_str(),res,regexNodes) ){

			int i;
			for (i =1; i < 13 && !res[i].matched; i ++);
			
			switch(i){
			case 1:		//while
				cmd.reset( new NodeWhile(tf) );		break;
			case 2:		//dowhile
				cmd.reset( new NodeDoWhile(tf) );		break;
			case 3:		//for
				cmd.reset( new NodeFor(tf) );		break;
			case 4:		//if
				cmd.reset( new NodeIf(tf) );		break;
			case 5:		//elsif
				cmd.reset( new NodeElsif(tf) );		break;
			case 6:		//elseif
				cmd.reset(new ErrorNode(tf,"'elseif' Node does not exist, try 'elsif'",tf->startIndex));
			case 9:		//endif
				cmd.reset( new NodeEndIf(tf) );		break;
			case 10:	//endfor
				cmd.reset( new NodeEndFor(tf) );		break;
			case 11:	//endwhile
				cmd.reset( new NodeEndWhile(tf) );		break;
			case 7:		//switch
			case 8:		//case
			case 12:	//endswitch
			case 13:	//endcase
				cmd.reset(new ErrorNode(tf,"Not yet implmented",tf->startIndex));
				break;
			default:	//should never get to this code
				cerr<<"CODE ERROR @ Node::createNode"<<endl;
				exit(0);
			}
			

		} else cmd.reset(new ErrorNode(tf,std::string("Invalid Node (") + *tf->rawTextFragment + ")",tf->startIndex));

		return cmd;
	}


}	}	//end namespaces
