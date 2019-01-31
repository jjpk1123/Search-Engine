#include <Corpus.h>
#include <Document.h>
#include <iostream>
#include <fstream>
#include <sstream>
using std::ifstream;

//Main class of CS253 semester project.
//Current version: PA8

/*This version will do the following:
	(1) A lot...
*/
///Code is what, comments are why!///

// Print the correct usage in case of user error
int Usage(char* arg0, int x)
{
	if (x == 0){
		cerr << "Usage: " << arg0 << " <doc to compare> <file w/ filenames> <low reading num> <high reading num>" << endl;
	}	
	else if (x == 1){
		cerr << "Cannot read file, ifstream failure" << endl;
	}
	else if (x == 4){
		cerr << "Cannot write file." << endl;
	}
	else if (x == 2){
		cerr << "Incorrect format of file" << endl;
	} 
	else if (x == 3){
		cerr << "Sorting error" << endl;
	} 
	else if (x == 5){
		cerr << "Scanning error" << endl;
	} 
	else if (x == 6){
		cerr << "Stemming error" << endl;
	} 
	else if (x == 7){
		cerr << "Invalid range" << endl;
		return Usage(arg0, 0);
	}
	else {
		cerr << "Generic error message" << endl;
	}
	return -1;
}

int main(int argc, char* argv[])
{	
	if (argc != 5) return Usage(argv[0], 0);
	
///---- set up file2 ----///
	Document file2;			//The file containing exceptions, and then the files for the corpus
	//Organize file2 in order to pass it over to ReadAll!!!
	ifstream istrFile2(argv[2]);
	if (istrFile2.fail()) return Usage(argv[0], -1);
	file2.ReadFile2(istrFile2);
	
	//Convert it to char* file2docs
	const char* file2docs[file2.words.size()];
	for (int i = 0 ; i < int(file2.words.size()) ; i++){
		file2docs[i] = file2.words[i].c_str();
	}
	
///---- set up file1 ----///
	//Grab exceptions file for file1
	const char* exceptionsFile = file2.words[0].c_str();
		
	//Open exceptions file
	ifstream istrExceptions(exceptionsFile);
	if (istrExceptions.fail()) return Usage(argv[0], -1);
	
	//Open document file1
	ifstream istrDocument(argv[1]);
	if (istrDocument.fail()) return Usage(argv[0], -1);
	
	//Create file1 Document
	Document file1;			//The file which we will compare to the corpus
	
	//Read the contents of the EXCEPTIONS FILE, line by line, making sure each has two words	
	if (istrExceptions.peek() == -1){}//Check if the file is empty, which is valid
	else if (!file1.ReadExceptions(istrExceptions)) return Usage(argv[0], -1);
	
	//Read the contents of the DOCUMENT FILE, delimiting whitespace, into a vector of strings
	if (!file1.ReadDoc(istrDocument)) return Usage(argv[0], -1);
	
///---- set up reading rangebow ----///
	double lo;
	double hi;
	istringstream is3(argv[3]);
	istringstream is4(argv[4]);
	is3 >> lo;
	is4 >> hi;
	if (is3.fail() || is4.fail())	return Usage(argv[0], -1); //If either value is not a double
	if (lo > hi) 					return Usage(argv[0], 7); //If the range is, like, 1-1 or 3-2 it's invalid
	
///---- set up corpus from file2 words ----///
	Corpus c0;				//Corpus of files to compare to file1
	//Populate corpus with documents from list provided to us by file2
	//file2.WriteDoc(cout);
	if (!c0.ReadAll(int(file2.words.size()), file2docs)){return Usage(argv[0], -1);}
	c0.docList.push_back(file1);		//The last file in the corpus is the query document
	if (!c0.ProcessAll(c0.docList.size())){return Usage(argv[0], -1);}
	
	//Create too many tfidf's!!!
	if (!c0.TFIDFAll()){return Usage(argv[0], -1);}
	
	/* for (int i = 0 ; i < int(c0.docList.size()) ; i++){
		cout << "doc[" << i << "]: " << c0.readingLevel[i] << endl;
	} */
	
	//Figure out which has the highest sim within reading rangebow!!!!!!
	double max = 0;
	int docIndex = -1;
	for (int i = 0 ; i < int(c0.docList.size())-1 ; i++){
		//If doc is within range
		if (c0.readingLevel[i] > lo && c0.readingLevel[i] < hi){
			double similarity = c0.sim(i, c0.docList.size()-1);
			//cout << "doc[" << i << "]sim: " << similarity << endl;
			if (similarity > max){
				max = similarity;
				docIndex = i;
			}
		}
	}
	if (docIndex == -1){
		cout << "There are no documents within the Coleman-Liau index." << endl;
		return -1;
	}else{
		cout << file2.words[docIndex+1] << endl;
	}
	return 0;
}
































