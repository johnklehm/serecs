#pragma once

#include "inc.h"

namespace serecs{

	/**
	 *	Helper class.  Used to store information about different types of tags
	 */
	class ParserSymbol{
	public:
		ParserSymbol(){
			this->symbol = '\0';
			this->requireClosing = true;
			this->newlinesAllowed = false;
		}

		bool operator==(ParserSymbol& other){
			return symbol == other.symbol;
		}

		char symbol;
		bool requireClosing;
		bool newlinesAllowed;

		friend class ParserOptions;
		static ParserSymbol none;
	};
	
	/**
	 *	Class to hold various options used when parsing the document.
	 *		Contains the start/end tags to look for and the various symbols
	 *		that represent the different types of tags
	 */
	class ParserOptions{
	public:
		ParserSymbol symControlBlock,
			symOutput,
			symConstant,
			symRawCode,
			symComment,
			symDirective;

		std::string startTag;
		std::string endTag;
			
		/**
		 *	Constructor.  Intitialize everything to use the default start
		 *		tags and various tag symbols
		 */
		ParserOptions() {

			symControlBlock.symbol = ':';
			symRawCode.symbol = '~';
			symRawCode.newlinesAllowed = true;
			symComment.symbol = '*';

			symDirective.symbol = '_';
			symDirective.requireClosing = false;

			symConstant.symbol = '_';
			symConstant.requireClosing = false;

			symOutput.symbol = '=';
			symOutput.requireClosing = false;

			startTag = "{{";
			endTag = "}}";
		}

		/**
		 *	Get the ParserSymbol associated with a symbol that's passed in
		 *	@return the ParserSymbol associated with a symbol. or ParserOptions::none if 
		 *		the association does not exist
		 */
		ParserSymbol& getSymbolType(char type){
			#define comp(tType,tSymbol) if (tType == tSymbol.symbol) return tSymbol;
			comp(type,symRawCode);
			comp(type,symOutput);
			comp(type,symControlBlock);
			comp(type,symDirective);
			comp(type,symComment);
			comp(type,symConstant);
			return ParserSymbol::none;
			#undef comp	
		}
	
	};



}
