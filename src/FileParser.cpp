
#define BOOST_FILESYSTEM_VERSION 3

#include "inc.h"
#include "FileParser.h"

#include <boost/filesystem.hpp>

namespace serecs{

	using namespace std;
	using namespace std::tr1;
	using namespace Nodes;
	
	ParserSymbol ParserSymbol::none;

	/**
	 *	Parse the entire document with default options
	 *	@see FileParser::parse(ParserOptions &opt)
	 */
	void FileParser::parse(){	
		parse(ParserOptions());	//parse with default options
	}

	/**
	 *	Parse an entire document checking template tags and correct syntax of the document
	 *	@param opt the options to use when parsing
	 */
	void FileParser::parse(ParserOptions &opt){	

		if (!getFileContents() || this->contents.empty()){
			cerr<<"File Cannot Be Found"<<endl;		//todo error
		} else {
			SharedTextFragmentVector document = createList();
			errors.output(cout,this->contents);
		}	
	
		return;
	}


	/**
	 *	Parse the document generating a list of all of the elements
	 *		including Nodes and text
	 *	@return the list of all the elements
	 */
	SharedTextFragmentVector FileParser::createList(){

		shared_ptr<vector<shared_ptr<TextFragment> > > document( new vector<shared_ptr<TextFragment> >);

		ParserOptions opt;
		int newlineIndex = -1;

		int startTagSize = opt.startTag.length(), endTagSize = opt.endTag.length();
		int startIndex = 0, endIndex = -startTagSize;

		shared_ptr<TextFragment> tf;
		shared_ptr<string> text;

		do {
			//find the next start tag
			startIndex = contents.find(opt.startTag,endIndex + startTagSize);
			if (startIndex == string::npos) break;	//no more, exit loop
			
			//add new text node:
			text.reset( new string( contents.substr(endIndex + startTagSize,startIndex - endIndex - startTagSize) ) );
			tf.reset( new TextFragment(text,0,endIndex + startTagSize, startIndex ) );
			document->push_back(tf);
			
			ParserSymbol type = opt.getSymbolType( contents[startIndex + startTagSize] );
			
			if (type == ParserSymbol::none){	//the symbol is undefined, throw an error
				errors.addError(string("Undefined tag identifier (") + contents[startIndex + startTagSize] + ")",startIndex+startTagSize);
				endIndex = startIndex;	continue;
			}	
			
			//check to see if there is a newline before we reach the end tag, if so throw an error
			newlineIndex = type.newlinesAllowed ? -1 : contents.find('\n',startIndex + startTagSize+1);		
			endIndex = contents.find(opt.endTag,startIndex + startTagSize+1);
			
			//is there a newline here?
			if (newlineIndex != -1 && newlineIndex < endIndex){
				errors.addError("Tag cannot contain newline",startIndex,newlineIndex);
			}

			//have we not found an end tag?
			if (endIndex == string::npos){
				errors.addError("Unclosed Tag",startIndex);
				break;
			} else if (type.requireClosing && contents[endIndex - 1] != type.symbol){	//perhaps it doesn't match the open tag
				errors.addError("Improperly closed tag",startIndex,endIndex+endTagSize);
				continue;
			}
			
			//add a Node text node
			int offset = type.requireClosing ? 2 : 1;
			text.reset( new string( contents.substr(startIndex + startTagSize + 1, endIndex - startIndex - startTagSize - offset) ) );
			tf.reset( new TextFragment(text,type.symbol,startIndex + startTagSize + 1, endIndex - endTagSize - offset) );
			document->push_back(tf);
			cout<<startIndex<<" " <<endIndex<<endl;
		} while (true);


	

		return document;

	}

	shared_ptr<Node>  FileParser::createTree(SharedTextFragmentVector document){
	
		shared_ptr<Node> root(new Node);


		shared_ptr<TextFragment> cmd;
		std::vector<shared_ptr<TextFragment> >::iterator iter;
		shared_ptr<Node> curParent = root;

		for (iter = document->begin(); iter != document->end(); iter++){
			cmd = *iter;

			if (cmd->type == 0){
			
				cout<<cmd->startIndex<<":"<<cmd->endIndex<<endl;
			}
			//	cout<< *(cmd->rawTextFragment);
		}

	
		cmatch res;
		static regex regexNodeType("\\s*([a-z]+)");
		static regex regexFor("\\s*\\(([^{};]*;[^{};]*;[^{};]*)\\)\\s*$");
		static regex regexCondition("\\s*\\(([^{};]*)\\)\\s*$");
	
		string str = "   for \n(var x = (7;x=1;x++)";
		if (regex_search(str.c_str(), res, regexFor)){
			cout<<res[1]<<endl;
		} else
			cout<<res[1].matched<<endl;
	
		return root;
	}

	bool FileParser::getFileContents(){
		boost::filesystem::path p(this->fileName);
	
		if (boost::filesystem::exists(p) && boost::filesystem::is_regular_file(p)){
			ifstream file(p.c_str());
			if (file.is_open()){
				uintmax_t size = boost::filesystem::file_size(p);
				contents.reserve((int)size + 1);
				getline(file,contents,'\0');

				return true;
			} else return false;
	
		} else return false;
	}

	void FileParser::reparse(){

		int startIndex = 0,endIndex = 0;

		while (true) {



		}


	}


}
