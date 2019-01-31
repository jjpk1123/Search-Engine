#ifndef DOCUMENT_H_INCLUDE
#define DOCUMENT_H_INCLUDE


#include<iostream>	//Read/write functions
#include<sstream>
#include<fstream>	//Take in file
#include<vector>	//Vector for words
#include<string>	//Need string type 
#include<map>
using namespace std;

class Document{
	
	public:
	//Constructor
	//Default constructor makes empty vector
	Document(){
		vector<string> words;
		vector<int> counts;
		vector<string> exceptions;
	}
		  
	// Accessors
	bool WriteDoc(ostream& ostr) const;
	bool WriteExcep(ostream& ost) const;
	inline bool startsWithCapital(string& str) const{return (str.front() >= 'A' && str.front() <= 'Z');}
	bool isAlpha(string& str) const;
	bool isAlpha(char a) const;
	bool isPunct(string& str) const;
	bool isPunct(char a) const;
	bool isAcro(string& str) const;
	bool isEOS(string& str) const;
	bool containsDigits(string& str) const;
	bool containsCapitals(string& str) const;
	int  inExceptions(string& str) const;
	double CL(Document& doc) const;
	int countLetters(Document& doc) const;
	int countWords(Document& doc) const;
	
	// Mutators
	bool ReadDoc(istream& istr);
	bool ReadFile2(istream& istr);
	bool ReadExceptions(istream& istr);
    bool delimitPush(string& str);
	bool MarkAmbiguousCapitalization();
	bool FixAmbiguousCapitalization ();
	bool TrimPunct();
	bool Stem();
	bool Sort();
	bool ShrinkCount();
	
	
	//private:
	int numSentences;
	vector<string> words;
	vector<int> counts;
	vector<string> exceptions;
	
};
#endif //DOCUMENT_H_INCLUDE
