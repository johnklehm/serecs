
#include "templates_Parser.h"
#include "templates_Fragment.h"



namespace serecs {	namespace templates {

	ParserOptions Parser::defaultOptions;
	ParserSymbol ParserSymbol::none;
	const std::string S("");

	/**
	 *	Loop through parsing the contents into fragments of code/commands/text/etc.
	 *	@note This function doesn't actually verify that the code is correct, it only
	 *		parses it into the desired fragments (though as of rewrite 3, it does
	 *		verify that strings/regexs/parenthesis are closed
	 */
	void Parser::parse(){

		//make sure options have correct attributes
		this->options.normalize();

		int startIndex = 0, endIndex = 0;
		ListOfFragmentPointersPointer listOfFragments(new ListOfFragmentPointers);
		ParserSymbol type;		

		//loop through parsing all of the tags
		while (true){
			 
			startIndex = contents.find(options.startTag,endIndex);

			//is there a tag at all?  if not, get out of this loop!
			if (startIndex == std::string::npos)	break;
			else startIndex += options.startTagSize;

			//get the type of command
			type = options.getSymbolType( contents[startIndex] );

			if (type == ParserSymbol::none){
				errors->addError(S + "Undefined tag indentifier (" + contents[startIndex] + ")",startIndex,startIndex);
				endIndex = startIndex + 1;
				continue;
			} else {
				FragmentPointer fp = parseToEndTag(startIndex+1,type);
				listOfFragments->push_back(fp);
				endIndex = fp->end+1;
			}

		}

		listOfFragments->push_back( Fragment::makeText(endIndex,contents.length() - 1) );

		/*
		for (auto i = listOfFragments->begin(); i != listOfFragments->end(); i++){
			std::cout<< contents.substr((*i)->start,(*i)->end - (*i)->start + 1) <<std::endl;
		}*/

	}

	/**
	 *	Parse a fragment to the end of the tag
	 *	@param startIndex the index to start parsing from
	 *	@param type the symbol indicating the symbol type
	 *	@return a FragmentPointer holding the fragment data
	 */
	FragmentPointer Parser::parseToEndTag(int startIndex,ParserSymbol &type){
		const std::string lookFor = S + "\n\\\"';()" + options.endTag[0];
		
		bool invalid = false;

		int numExpressions = 0;
		std::stack<int> parens;

		int index = startIndex;
		int result;
		while (!invalid){
			
			index = contents.find_first_of(lookFor,index);
			//if we found the end of the content
			if (index == std::string::npos){
				index = contents.length();
				errors->addError("Missing end tag",startIndex,index);
				invalid = true;
				break;
			//if we found the end tag
			} else if (contents[index] == options.endTag[0]){
				result = checkEndTag(startIndex,index,type);
					
				if (result == END_NOTEND){
					index++;	
					continue;
				} else if (result == END_INVALID1 || result == END_INVALID2){
					//from the start to the end index - endCharacter - (0/1 depending if it had the end symbol)
					index += -1 - (result + 2);
					break;
				} else {
					index -= 1 + result ;
					break;
				}
				
				

			//not the end or an end tag, test for other things
			} else {
			
				switch (contents[index] ){
				case '\n':
					if (!type.newlinesAllowed){
						errors->addError("Invalid newline (not allowed in this tag type)",index,index);
						invalid = true;
					}
					break;
				case '\\':
					result = parseTillEndOfRegex(startIndex,index,type);
					if (result < 0){
						index = -result;
						invalid = true;
					} else 
						index =result;
					break;
				case '\"':
				case '\'':
					result = parseTillEndOfString(startIndex,index,type);
					if (result < 0){
						index = -result;
						invalid = true;
					} else 
						index =result;
					break;
				case '(':
					parens.push(index);
					break;
				case ')':
					if (parens.empty()){
						errors->addError("Unexpected closing parenthesis (is it extra?)",index,index);
						invalid = true;
					} else {
						parens.pop();
					}
					break;
				case ';':
					numExpressions++;
					break;
				}
				index++;
			}
		}//end while

		if (!parens.empty()){
			errors->addError("Unclosed Parenthesis",parens.top(),index);
			invalid = true;
		} 

		//if it's valid, return the fragment, with the correct type, otherwsie return INVALID type
		if (!invalid) return Fragment::make(startIndex,index,options.getFragmentType(type),numExpressions);
		else return Fragment::make(startIndex,index,Fragment::INVALID,numExpressions);
	}

	/**
	 *	Check to see if the content starting at index is a valid end tag
	 *	@param startIndex the start of the current tag (for error reporting)
	 *	@param index the index to start looking for an end tag
	 *	@param type the type of symbol we're looking for
	 *	@returns END_INVALID1 or END_INVALID2 if it is improperly closed, END_NOTEND
	 *		if it is not an end tag, or and offset (0 or 1) to subtract from the index
	 *		to get the content from the tag
	 *
	 *	@notes adds errors to the document
	 */
	int Parser::checkEndTag(const int startIndex,int index,ParserSymbol &type){

		if (contents.substr(index,options.endTagSize) == options.endTag){	//do we have an end tag
			bool hasCloseSymbol = contents[index-1] == type.symbol;				//does it close with a symbol

			//it ends after the tag
			index += options.endTagSize;
			if (type.requireClosing == ParserSymbol::REQUIRED && !hasCloseSymbol){		//were we supposed to have a symbol, but don't?
				errors->addError(S + "Invalid closing tag (expected '" + type.symbol +options.endTag+ "')",startIndex - options.startTagSize,index);
				return END_INVALID1;
			} else if (type.requireClosing == ParserSymbol::EMPTY && hasCloseSymbol){	//were we not supposed to hae symbol, but do?
				errors->addError(S + "Invalid closing tag (end tag should not consist of symbol '" + type.symbol + "')",startIndex - options.startTagSize,index);
				return END_INVALID2;
			} else return hasCloseSymbol ? 1 : 0;				//we're good, return offset of 1 if we have the symbol, else none
		} else return END_NOTEND;					//we don't have an end tag at all. 
	}

	/**
	 *	Parse until the end of javascript string literal
	 *	@param startIndex the start of the current tag (for error reporting)
	 *	@param index the index to of the begining of the string (should be ' or ")
	 *	@param type the type of symbol we're looking for (to check if newlines are allowed)
	 *	@returns negative (index) if the entire tag should end (errors already reported)
	 *		positive (index) for the end character of the string (' or ")
	 *
	 *	@notes adds errors to the document
	 */
	int Parser::parseTillEndOfString(const int startIndex,int index,ParserSymbol &type){
		const std::string lookFor(S + "\n\\" + this->contents[index]);
		const int stringStart = index;
		const int max = contents.length();
		bool invalid = false;
		bool continueSearch = true;

		while (continueSearch){
			index++;
			index = this->contents.find_first_of(lookFor,index);
			
			if (index == std::string::npos){
				index = max - 1;		//make sure index is correct for error reporting
				errors->addError("Unterminated string",stringStart,index);
				errors->addError("Unterminated tag",startIndex,index);
				invalid = true;
				continueSearch = false;
			} else {
				switch(this->contents[index]){
				case '\n':		//strings cannot have newlines unless preceded by escape character
					errors->addError("Invalid newline in string (this type of tag cannot contain a newline)",stringStart,index);
					errors->addError("Unterminated start tag",startIndex,index);
					invalid = true;
					continueSearch = false;
					index += 1; //get to index after newline
					break;
				case '\\':		//escape character (ignore next character), unless it's a newline
					//if we're allowed to have newlines check to see if it's a newline and 
					if (!type.newlinesAllowed && index+1 < max && contents[index+1] == '\n'){
						errors->addError("Invalid newline in string (this type of tag cannot contain a newline [even one preceded by a backslash])",stringStart,index);
						errors->addError("Unterminated start tag",startIndex,index);
						invalid = true;
						continueSearch = false;
					}

					index++;	//increment passed the escape character (if it's a multicharacter escape sequence, we can parse it normally
									
					break;
				default:	//end string '/"
					continueSearch = false;	//we found the end!
					break;
				}
			}	//end if
		}	//end while

		if (invalid)	return -index;
		else			return index;
	}

		

	int Parser::parseTillEndOfRegex(const int startIndex,int index,ParserSymbol &type){

		const std::string lookFor(S + "\n\\" + this->contents[index]);
		const int stringStart = index;
		const int max = contents.length();
		bool invalid = false;
		bool continueSearch = true;

		while (continueSearch){
			index++;
			index = this->contents.find_first_of(lookFor,index);
			
			if (index == std::string::npos){
				index = max - 1;		//make sure index is correct for error reporting
				errors->addError("Unterminated regex",stringStart,index);
				errors->addError("Unterminated tag",startIndex,index);
				invalid = true;
				continueSearch = false;
			} else {
				switch(this->contents[index]){
				case '\n':		//strings cannot have newlines unless preceded by escape character
					errors->addError("Invalid newline in regex (regexs cannot contain newlines)",stringStart,index);
					errors->addError("Unterminated start tag",startIndex,index);
					invalid = true;
					continueSearch = false;
					index += 1; //get to index after newline
					break;
				case '\\':		//escape character (ignore next character), unless it's a newline
					//if we're allowed to have newlines check to see if it's a newline and 
					if (!type.newlinesAllowed && index+1 < max && contents[index+1] == '\n'){
						errors->addError("Invalid newline in regex (regexs cannot contain newlines)",stringStart,index);
						errors->addError("Unterminated start tag",startIndex,index);
						invalid = true;
						continueSearch = false;
					}

					index++;	//increment passed the escape character (if it's a multicharacter escape sequence, we can parse it normally
									
					break;
				default:	//end string '/"
					continueSearch = false;	//we found the end!
					break;
				}
			}	//end if
		}	//end while

		if (invalid)	return -index;
		else			return index;
	}

	


}	}		//end namespace serecs::templates::