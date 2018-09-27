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
  string name;
	string lexemeName;
	string token;
  // TODO
};

struct BNF
{
  string pattern;
  string name;
};

// Prototypes
vector<Token> Lexer(string expression);

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
 
  // use a loop to scan each line in the file
  while(getline(infile, expression))
    {
      tokens = Lexer(expression);
      
      // display the tokens to the screen
      for(unsigned x = 0; x < tokens.size(); ++x)
        {
      	  cout<<tokens[x].lexemeName<<"  t"
      	      <<tokens[x].token<<endl;
        }
    }
 
  infile.close();
  
  return 0;
}

vector<Token> Lexer(string expression)
{
    vector<Token> tokens;

    // Defining regular expressions. RegEx in C++: http://www.informit.com/articles/article.aspx?p=2064649&seqNum=3
    vector<BNF> patterns
    {
      { "#.*" ,   "COMMENT" },
	{"\\bprint\\b|\\bif\\b|\\belse\\b|\\bFalse\\b|\\bNone\\b|\\band\\b|\\bor\\b|\\bfor\\b|\\bin\\b",   "RESERVED WORD"},
	  { "\".*\"|\'.*\'" ,   "STRING" },
	    { "([a-z]|[A-Z]|_)([a-z]|[A-Z]|[0-9]|_)*" ,   "IDENTIFIER" },
	      { "[0-9]+" ,   "NUMBER" },
		{ "\\*|\\+|==|=|-",  "OPERATOR" },
			{"\\(|\\{", "START_GROUP"},
			{"\\)|\\}", "END_GROUP"},
			{"\\:", "BLOCK_START"},
			{"\t", "TAB"}

    };

    // storage for results
    map< size_t, pair<string,string> > matches;
    size_t index = 0;
    cout << expression << endl;
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

    for(auto match = matches.begin(); match != matches.end(); ++match)
      cout<< "<" << match->second.first << ", " << match->second.second << "> ";
    
    expression = regex_replace(expression, regex("^ +| +$|( ) +"), "$1");
    if(expression.size())
      {
	cout << "<" << expression << ", ERROR>";
      }
    
    cout << endl << endl;
    return tokens;
}// end of Lexer
