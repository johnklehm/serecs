
#pragma once;

#include "inc.h"
#include "templates_Fragment.h"
#include "templates_ParserOptions.h"
#include "Errors.h"


namespace serecs {	namespace templates {

	class Parser;
	typedef std::tr1::shared_ptr<Parser> ParserPointer;

	class Parser{
		struct ParseData{
			int numExpressions, endIndex;
			bool invalid;
		};

	public:
		static ParserOptions defaultOptions;
		ErrorDocumentPointer errors;
		std::string contents;

		Parser(ErrorDocumentPointer edp,std::string &theContents, ParserOptions po = defaultOptions){
			errors = edp;
			contents = theContents;
			options = po;
		}

		void parse();

		//TODO: Improve
		static ParserPointer fromFile(std::string filename, ParserOptions po = defaultOptions){
			ErrorDocumentPointer edp( new ErrorDocument(filename) );
			boost::filesystem::path p(filename);
			std::string contents;
			std::ifstream file(p.c_str());		
			uintmax_t size = boost::filesystem::file_size(p);
			contents.reserve((int)size + 1);
			std::getline(file,contents,'\0');

			return ParserPointer( new Parser(edp,contents) );
		}

	private:
		FragmentPointer parseToEndTag(int startIndex,ParserSymbol &type);
		int checkEndTag(int startIndex,int index,ParserSymbol &type);
		int parseTillEndOfString(int startIndex,int index,ParserSymbol &type);
		int parseTillEndOfRegex(int startIndex,int index,ParserSymbol &type);
		

	private:
		
		ParserOptions options;		

		static const int END_NOTEND		= -1;
		static const int END_INVALID1	= -2;
		static const int END_INVALID2	= -3;

	};


}	}		//end namespace serecs::templates::