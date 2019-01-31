#ifndef CORPUS_H_INCLUDE
#define CORPUS_H_INCLUDE

#include<Document.h>
#include<iostream>	//Read/write functions
#include<sstream>
#include<fstream>	//Take in file
#include<vector>	//Vector for words
#include<string>	//Need string type 
using namespace std;

class Corpus{
	
	public:
	//Constructor
	Corpus(){
		vector<Document> docList;
		vector<vector<double>> TFIDF;
		vector<vector<double>> simMatrix;
		vector<double> idf;
		vector<double> readingLevel;
	}
		  
	// Accessors
	double calcIDF(string& word); //Look through all docs and ask if word appears
	bool WriteSimMatrix(ostream& ostr) const;
	
	//Mutators
	bool ReadAll (int argc, const char** argv); //Populate the docList data structure
	bool ReadDoc (int n, const char** argv); //Read one document file 
	bool ProcessAll (int numDocs);
	bool ProcessDoc (Document& e1);
	bool TFIDFAll ();
	bool TFIDFDoc (Document& doc);
	bool calcSimMatrix();
	double sim(int i, int j);
	
	//private:
	vector<Document> docList;
	vector<vector<double>> TFIDF;
	vector<vector<double>> simMatrix;
	vector<double> idf;
	vector<double> readingLevel;
};
#endif //CORPUS_H_INCLUDE