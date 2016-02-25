/*
 * Author: Xiakun Lu (CWID: 10396542)
 *
 * I have finished the extra credit:
 * The vector can be found in the class calculator and named _symbols.
 * The convert process can be found in the expression2symbol function.
 *
 * For 15 points of extra credit, store all symbols for 
 * each line in a vector of symbol and then do data processing 
 * on each line only after it has been completely read in. 
 * You will need to figure out whether you have an operand or 
 * operator as you loop through this vector, so research 
 * dynamic_cast to perform that function.
 */


#include <iostream>
#include <fstream>
#include "symbol.h"

using namespace std;

int main()
{
	string path("");
	cout << "Input the location of an input text file:" << endl;
	cin >> path;

	ifstream infile;
	infile.open(path);
	while (!infile) {
		cout << "Wrong location, input again." << endl;
		cin >> path;
		infile.open(path);
	}

	string exp("");
	while (infile.good() && !infile.eof() && EOF != infile.peek()) {
		getline(infile, exp);
		if (exp.length() == 0) continue;

		calculator calc;
		try {
			cout << calc(exp) << endl;
		}
		catch (exception &e) {
			cout << "SYNTAX ERROR" << endl;
			//cout << "SYNTAX ERROR: " << e.what() << endl;
		}
	}
	return 0;
}