#include<Corpus.h>
#include<Document.h>
#include<math.h>
#include <iomanip>

bool Corpus::ReadAll (int numDocs, const char* dox[]){
	for (int n = 1 ; n < numDocs ; n++){
		if (!ReadDoc(n, dox)){return false;}
	}
	return true;
}
	
bool Corpus::ReadDoc(int n, const char* dox[]){
	if (!(n >= 1)){return false;} //Function only works for n greater than 1, as docList[0] is exceptions file
	
	//Open exceptions file
	ifstream istrExceptions(dox[0]);
	if (istrExceptions.fail()) return false;
	
	//Open document n
	ifstream istrDocument(dox[n]);
	if (istrDocument.fail()) return false;
	
	//Create Document object
	Document d0;
	
	//1a. Read the contents of the EXCEPTIONS FILE, line by line, making sure each has two words	
	if (istrExceptions.peek() == -1){}//Check if the file is empty, which is valid
	else if (!d0.ReadExceptions(istrExceptions)) return false;
	
	//1b. Read the contents of the DOCUMENT FILE, delimiting whitespace, into a vector of strings
	if (!d0.ReadDoc(istrDocument)) return false;
	
	//We're finished, add it to docList
	docList.push_back(d0);
	return true;
}

bool Corpus::ProcessAll (int numDocs){
	for (int n = 0 ; n < numDocs ; n++){
		if (!ProcessDoc(docList[n])){return false;}
	}
	return true;
}

bool Corpus::ProcessDoc (Document& e1){
	if (!e1.MarkAmbiguousCapitalization()) return false;
	if (!e1.FixAmbiguousCapitalization()) return false;
	double RL = e1.CL(e1);
	if (RL == -999) return false;
	readingLevel.push_back(RL);
	if (!e1.Stem()) return false;
	if (!e1.TrimPunct()) return false;
	if (!e1.Sort()) return false;
	if (!e1.ShrinkCount()) return false;
	return true;
}

bool Corpus::TFIDFAll(){
	//Iterate through each doc
	for (int i = 0 ; i < int(docList.size()) ; i++){
		//Create vector to be pushed onto TFIDF vector at index of the doc
		vector<double> TFIDFScores; 
	
		//Iterate through the words of the doc
		for (int j = 0 ; j < int(docList[i].words.size()) ; j++){
			int TF = docList[i].counts[j]; //Get count for the word
			double IDF = calcIDF(docList[i].words[j]);
			double tfidf = TF*IDF;
			TFIDFScores.push_back(tfidf);
		}
		TFIDF.push_back(TFIDFScores);
	}
	return true;
}

double Corpus::calcIDF(string& word) {
	double count = 0.0;
	for (int i = 0 ; i < int(docList.size()) ; i++){ 		//Look at each doc
		for (int j = 0 ; j < int(docList[i].words.size()) ; j++){ //Look at each word
			if (docList[i].words[j] == word){				//Is the word in there?
				//Increment count, safe because you look for a word in a doc, can match with same doc. Always >0
				count = count + 1.0;	
				break;										//Go to next doc
			}
		}
	}
	double idf = log10(docList.size()/count);
	return idf;
}

bool Corpus::calcSimMatrix(){
	for (int i = 0 ; i < int(docList.size()) ; i++){ //Rows
		vector<double> row (int(docList.size()), 0); //Fill row with zeroes initially, no push_back
		for (int j = 0 ; j < int(docList.size()) ; j++){ //Cols
			row[j] = sim(i,j); //row[j] = simMatrix[i][j]
		}
		simMatrix.push_back(row);
	}
	return true;
}

double Corpus::sim(int doc1Index, int doc2Index){
	Document& doc1 = docList[doc1Index];
	Document& doc2 = docList[doc2Index];
	double similarity = 0.0;
	//Go through every word in doc1
	for (int i = 0 ; i < int(doc1.words.size()) ; i++){
		//Go through every word in doc2 until we find a match
		for (int j = 0; j < int(doc2.words.size()) ; j++){
			//Ask, is it in doc2?
			if (doc1.words[i] == doc2.words[j]){
				double simScore = TFIDF[doc1Index][i] * TFIDF[doc2Index][j];
				similarity = similarity + simScore;
				//break; //Go to next word in doc1
			}
		}
	}
	return similarity;
}

bool Corpus::WriteSimMatrix(ostream& ostr) const{
	for (int i = 0 ; i < int(simMatrix.size()) ; i++){
		for (int j = 0 ; j < int(simMatrix[i].size()) ; j++){
			ostr << fixed;
			ostr << setprecision(5) << simMatrix[i][j] << " ";
		}
		ostr << endl;
	}
	return true;
}




















