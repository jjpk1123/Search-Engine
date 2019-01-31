#ifndef STEMMER_H_INCLUDE
#define STEMMER_H_INCLUDE


#include<iostream>	//Read/write functions
#include<fstream>	//Take in file
#include<vector>	//Vector for words
#include<string>	//Need string type 
using namespace std;

class Stemmer{
	public:
	Stemmer(string* toBeStemmed)
		:word(toBeStemmed)
		{}
	
	//Accessors
	inline string getWord() const{return *word;}
	bool containsVowel(string str) const;
	bool isVowel(string& str, int i) const;
	bool isDouble(string& str) const;
	bool isValidLiEnding(char a) const;	
	bool endsInShortSyllable(string& str) const;
	bool isShortWord(string& str) const;
	bool suffixIs(string& str, string suffix) const;
	string region1(string& str, string r1) const;
	string region2(string& str) const;
	string preceder(string& str, string suffix) const;
	
	//Mutators
	bool replaceSuffix(string& str, string suffix, string replacement);
	
	//Porter Steps 1-8
	bool porter();
	bool step1();
	bool step2();
	bool step3();
	bool step4();
	bool step5();
	bool step6();
	bool step7();
	bool step8();
	
	private:
	string* word;
};

#endif //STEMMER_H_INCLUDE