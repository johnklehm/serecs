
#pragma once

#include "inc.h"

#include "templates_Fragment.h"

namespace serecs {	namespace templates {

	/**
	 *	Helper class.  Used to store information about different types of tags
	 */
	class ParserSymbol{
	public:
		ParserSymbol(){
			this->symbol = '\0';
			this->requireClosing = ALLOWED;
			this->newlinesAllowed = false;
		}

		bool operator==(ParserSymbol& other){
			return symbol == other.symbol;
		}

		char symbol;
		int requireClosing;
		bool newlinesAllowed;

		friend class ParserOptions;
		static ParserSymbol none;

		static const int REQUIRED	= 0;
		static const int ALLOWED	= 1;
		static const int EMPTY		= 2;
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
			symRawCode,
			symComment,
			symPlugin;

		std::string startTag;
		std::string endTag;
		int startTagSize;
		int endTagSize;
			
		/**
		 *	Constructor.  Intitialize everything to use the default start
		 *		tags and various tag symbols
		 */
		ParserOptions() {

			symRawCode.symbol = '~';
			symRawCode.newlinesAllowed = true;
			symRawCode.requireClosing = ParserSymbol::REQUIRED;

			symControlBlock.symbol = ':';
			symComment.symbol = '*';
			symPlugin.symbol = '#';
			symOutput.symbol = '=';

			startTag = "<%";
			endTag = "%>";
		}

		/**
		 *	Get the ParserSymbol associated with a symbol that's passed in
		 *	@return the ParserSymbol associated with a symbol. or ParserOptions::none if 
		 *		the association does not exist
		 */
		ParserSymbol& getSymbolType(char type){
			#define comp(tSymbol) if (type == tSymbol.symbol) return tSymbol;
			comp(symRawCode);
			comp(symOutput);
			comp(symControlBlock);
			comp(symPlugin);
			comp(symComment);
			return ParserSymbol::none;
			#undef comp	
		}

		int getFragmentType(ParserSymbol &type){
			using namespace serecs::templates;
			#define comp(tType,value) if (type == tType) return value
			comp(symRawCode,Fragment::CODE);
			comp(symOutput,Fragment::OUTPUT);
			comp(symControlBlock,Fragment::BLOCK);
			comp(symPlugin,Fragment::PLUGIN);
			comp(symComment,Fragment::COMMENT);
			return Fragment::UNKNOWN;
			#undef comp	
		}

		void normalize(){
			startTagSize = startTag.length();
			endTagSize = endTag.length();
		}
	
	};



}	}		//end namespace serecs::templates::
