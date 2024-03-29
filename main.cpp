#include <iostream>
#include <cstdlib>
#include <cctype>
#include <string>
#include <vector>
#include <fstream>
#include <regex>
#include <list>
#include <map>
#include <set>

using namespace std;

struct Token
{
  string lexemeName;
  string token;
  int line;
  int linePosition;

  void out(){
    cout << lexemeName << " " << token << " " << line << " " << linePosition << endl;
  }
};

struct BNF
{
  string pattern;
  string name;
};

// Prototypes
vector<Token> Lexer(string expression, int line);

int main(int argc, char *argv[])
{
  // variable declaration
  ifstream infile;
  string fileName = "";
  string expression = "";
  vector<Token> tokens;
 
  // Reading the data from the user
  cout<<"Please enter the name of the file: ";
  getline(cin, fileName);
  
  infile.open(fileName.c_str());

  if(infile.fail())
    {
      cout<<"** ERROR - the file ""<<fileName<<"" cannot be found!\n";
      exit(1);
    }
  int line = 1;
  // use a loop to scan each line in the file

  map<size_t, Token> SymbolTable;
  size_t index = 0;
  while(getline(infile, expression))
    {
      tokens = Lexer(expression, line);
      line++;
      // display the tokens to the screen
      for(unsigned x = 0; x < tokens.size(); ++x)
        {
	  tokens[x].linePosition = expression.find(tokens[x].token);
	  SymbolTable[index] = tokens[x];
	  index++;
        }
    }
 
  infile.close();

  // Printing the Symbol Table

  cout << "----------Symbol Table------------" <<  SymbolTable.size() << endl; 
  for(auto row = SymbolTable.begin(); row != SymbolTable.end(); ++row){
    row->second.out();
  }
  
  return 0;
}

vector<Token> Lexer(string expression, int line)
{
    vector<Token> tokens;

    // Defining regular expressions. RegEx in C++: http://www.informit.com/articles/article.aspx?p=2064649&seqNum=3
    vector<BNF> patterns
    {	{ "#.*" ,   "COMMENT" },
	{"\\bprint\\b|\\bif\\b|\\belse\\b|\\bFalse\\b|\\bNone\\b|\\band\\b|\\bor\\b|\\bfor\\b|\\bin\\b",   "RESERVED WORD"},
	  { "\".*\"|\'.*\'" ,   "STRING" },
	    { "([a-z]|[A-Z]|_)([a-z]|[A-Z]|[0-9]|_)*" ,   "IDENTIFIER" },
	      { "[0-9]+" ,   "NUMBER" },
		{ "\\*|\\+|==|=|-",  "OPERATOR" },
			{"\\(|\\{", "START_GROUP"},
			{"\\)|\\}", "END_GROUP"},
			{"\\:", "BLOCK_START"}
    };
    cout << expression << endl;
    while(true){
      int tab = 1;
      auto position = expression.find('\t');
      if(position != 0){
	position = expression.find("    ");
	tab = 0;
      }
      
      if(position == 0){
	Token token = Token();
	token.lexemeName = "TAB";
	token.line = line;
	if(tab){
	  expression.replace(0,1,"");
	  token.token = "\t";
	}	
	else{
	  expression.replace(0,4,"");
	  token.token = "    ";
	}
	cout << "<\t, TAB> ";  
	tokens.push_back(token);
      }
      else{
	break;
      }
    }
    
    // storage for results
    map< size_t, pair<string,string> > matches;
    size_t index = 0;

    for(auto pat = patterns.begin(); pat != patterns.end(); ++pat)
      {
	regex r(pat->pattern);
	auto words_begin = sregex_iterator( expression.begin(), expression.end(), r );
	auto words_end   = sregex_iterator();
      
	for( auto token = words_begin; token != words_end; ++token){
	  size_t position = token->position();
	  string tokenTmp = token->str();
	  if(matches.find(position + index) != matches.end() ){
	    index++;
	  }
	  matches[ position + index ] = make_pair( tokenTmp, pat->name );

	  size_t posFound = expression.find(tokenTmp);
	  if (posFound!=string::npos)
	    expression.replace(posFound, tokenTmp.size(),"");
	
	}
      }

    for(auto match = matches.begin(); match != matches.end(); ++match){
      cout<< "<" << match->second.first << ", " << match->second.second << "> ";
      Token foundT = Token();
      foundT.lexemeName = match->second.second;
      foundT.token = match->second.first;
      foundT.line = line;
      tokens.push_back(foundT);
    }
          
    expression = regex_replace(expression, regex("^ +| +$|( ) +"), "$1");
    if(expression.size())
      {
	cout << "<" << expression << ", ERROR>";
      }
    
    cout << endl << endl;
    return tokens;
}// end of Lexer
