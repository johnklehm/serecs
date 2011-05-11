

#include "inc.h"
#include "templates_Parser.h"
#include "Node.h"

#include "tests/Tests.h"

using namespace std;
using namespace std::tr1;



int main(){
	using namespace serecs;
	using namespace serecs::Nodes;

	serecs::templates::ParserPointer pp = serecs::templates::Parser::fromFile("S:/src/test.txt");
	//std::cout<<pp->contents<<std::endl;
	pp->parse();

	

	cout<<"errors:"<<endl;
	pp->errors->output(cout,pp->contents);

	_getch();
	return 0;


}
