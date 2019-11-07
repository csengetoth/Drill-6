#include "std_lib_facilities.h"

class Token
{
	public:
		char kind;
		double value;
};

class Token_stream
{
	public:
		Token_stream(); // make a Token_stream that reads from cin
		Token get(); // get a Token
		void putback(Token t); // put a Token back
	private: // implementation details
		bool full {false};
		Token buffer;
};

void Token_stream::putback(Token t)
{
	if (full) error("putback() into a full buffer");
	buffer=t; // copy t to buffer
	full=true; // buffer is now full
}

Token Token_stream::get()
{
	if(full) // do we already have a Token ready?
		{
				
			full=false; // remove Token from buffer
			return buffer;
		}
	char ch;
	cin>>ch; // note that >> skips whitespace (space, newline, tab, etc.)
	switch(ch)
		{
			case '=': // for “print”
			case 'x': // for “quit”
			case '(': case ')': case '+': case '*': case '/':
				return Token{ch}; // let each character represent itself
			case '.':
			case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
			{
				cin.putback(ch); // put digit back into the input stream
				double val;
				cin>>val; // read a floating-point number
				return Token{'8',val}; // let ‘8’ represent “a number”
			}
			default: 
				error ("Bad token");
		}
}

Token_stream ts; // provides get() and putback()

double expression(); // declaration so that primary() can call expression()

double primary()
{
	Token t=ts.get();
	switch(t.kind)
		{
			case '(':
				{
					double d=expression();
					t=ts.get();
					if(t.kind!=')') error("')' expected");
					return d;
				}
			case '8':
				return t.value;
			default:
				error("primary expected");
		}
}

double term()
{
	double left=primary();
	Token t=ts.get();
	while(true)
		switch(t.kind)
			{
				case '*':
					left*=primary();
					ts.get();
					break;
				case '/':
					{
						double d=primary();
						if (d==0) error ("divide by zero");
						left/=d;
						t=ts.get();
						break;
					}
				default: 
					return left;
			}
}

double expression()
{
	double left=term();
	Token t= ts.get();
	while(true)
		{
			switch (t.kind)
				{
					case '+':
						left+=term();
						t= ts.get();
						break;
					case '-':
						left-=term();
						t= ts.get();
						break;
						default:																				
						ts.putback(t);						
						return left;
				}
		}
}




int main()
{
	cout<<"Welcome to our simple calculator. Please enter expressions using floating-point numbers. Available operators are the following: +,-,*,/. Type = to print and x to exit."<<'\n';
	try
		{
			while(cin)
				cout<<expression()<<'\n';
		}
	catch(exception& e)
		{
			cerr<<e.what()<<'\n';
			return 1;
		}
	catch (...)
		{
			cerr<<"expection\n";
			return 2;
		}
	double val = 0;
	while (cin) 
		{
			Token t = ts.get();
			if (t.kind == 'x') break; // ‘q’ for “quit”
			if (t.kind == '=')	// ‘;’ for “print now”
				cout << "=" << val << '\n';
			else
				ts.putback(t);
			val = expression();
		}
}






	
