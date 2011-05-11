
#pragma once

#ifdef ADSDEFSDF

#include "inc.h"
#include <v8/v8.h>



class ParserVerification{
public:

	shared_ptr<std::string> errorText;
	int lineNumber,startPos,endPos;

	ParserVerification(){
		errorText.reset(new std::string);
		lineNumber = startPos = endPos = -1;
	}

	/**
	 *
	 *
	 */
	bool verify(shared_ptr<std::string> what){return verify(what->c_str());}
	bool verify(std::string &what){return verify(what.c_str());}
	bool verify(const char *what){
		using namespace v8;
		

		static HandleScope handle_scope;
		static Persistent<Context> context = Context::New();
		static Context::Scope context_scope(context);

		TryCatch tryCatch;

		Local<String> code = String::New(what);
		Handle<v8::Script> script = Script::Compile(code);

		if (script.IsEmpty()){	//report error
			*errorText = ToCString(v8::String::Utf8Value(tryCatch.Exception()));			
		

			Handle<v8::Message> message = tryCatch.Message();

			if (!message.IsEmpty()){
				this->lineNumber = message->GetLineNumber()-1;
				this->startPos = message->GetStartPosition();
				this->endPos = message->GetEndPosition();

				cout<<*errorText<<':'<<lineNumber<<'-'<<startPos<<','<<endPos<<endl;
			}
			return false;
		} else {				//no error!
			return true;
		}
	}

	

private:
	// Extracts a C string from a V8 Utf8Value.
	const char* ToCString(const v8::String::Utf8Value& value) {
		return *value ? *value : "<string conversion failed>";
	}


};

#endif
