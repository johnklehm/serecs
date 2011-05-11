
#pragma once

#include "inc.h"

namespace serecs{

	class GeneralError{

	public:
		GeneralError(std::string tErrorText,int tStartIndex,int tEndIndex, int tType){
			this->errorText = tErrorText;
			this->startIndex = tStartIndex;
			this->endIndex = tEndIndex;
			this->type = tType;
		}

		std::string errorText;
		int startIndex,endIndex;
		int type;

		static const int typeError = 0;
		static const int typeWarning = 8;
	};

	class ErrorDocument;
	typedef std::tr1::shared_ptr<ErrorDocument> ErrorDocumentPointer;

	class ErrorDocument{

		std::string filename;
		std::vector<GeneralError> errors;

	public:
		ErrorDocument(std::string tFilename){
			this->filename = tFilename;
		}

		void addError(std::string tText, int tStart, int tEnd = -1){
			errors.push_back( GeneralError(tText, tStart, tEnd,GeneralError::typeError) );
		}


		void addWarning(std::string tText, int tStart, int tEnd = -1){
			errors.push_back( GeneralError(tText, tStart, tEnd,GeneralError::typeWarning ) );
		}

		bool isEmpty(){
			return this->errors.empty();
		}

		void output(std::ostream &out){
			for (auto i = errors.begin(); i != errors.end(); i++){
				out<<'"'<<filename<<"\" "<<i->errorText<<":"<<i->startIndex<<","<<i->endIndex<<std::endl;
			}
		}

		void output(std::ostream &out,std::string &contents){
			for (auto i = errors.begin(); i != errors.end(); i++){
				LineInfo li = getLineNumber(contents,i->startIndex);
				out	<<'"'<<filename<<"\" "<<i->errorText
					<<": line "<<li.numLines<<", colomn "<<i->startIndex - li.lastLineIndex
					<<" length "<<i->endIndex - i->startIndex<<std::endl;
			}
		}

		struct LineInfo{
			int numLines;
			int lastLineIndex;
		};


		static LineInfo getLineNumber(std::string &contents,int index){
			int foundIndex = 0;
			int oldIndex = 0;
			int numLines = 0;

			while (foundIndex < index){
				oldIndex = foundIndex;
				foundIndex = contents.find('\n',foundIndex+1);
				numLines++;
			}

			LineInfo li = {numLines,oldIndex};

			return li;

		}

	};














}
