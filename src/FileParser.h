
#pragma once

#include "inc.h"
#include "Node.h"
#include "Errors.h"
#include "ParserOptions.h"

using namespace std;

namespace serecs{
	
	class FileParser{
	private:
		std::string fileName;
		std::string contents;
	public:

		FileParser(std::string theFileName) : errors(theFileName){
			this->fileName = theFileName;
		}

		ErrorDocument errors;

		void parse();
		void parse(ParserOptions &opt);
		void reparse();
		bool getFileContents();
		SharedTextFragmentVector createList();
		shared_ptr<Nodes::Node>  createTree(SharedTextFragmentVector document);

	};

}
