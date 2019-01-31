#include<Document.h>
#include<Stemmer.h>
#include<algorithm>

//ReadDoc
bool Document::ReadDoc (istream& istr) {  
	//While the input stream is working
	while (!istr.fail()){
		string nextWord;
		istr >> nextWord;
		//If it fails to read next word, check for eof
		if (istr.fail()){
			if (istr.eof())	{//If end of file...
				if (words.size() == 0){return false;} //No words
				return true;} 	 //Legal eof()
			else {return false;} //Illegal input, false
		}
		
		//Iterate through the word character by character, separating into multiple strings based on punc rules
		if (isAlpha(nextWord)){words.push_back(nextWord);} 		//All alpha word
		else if (isPunct(nextWord)){words.push_back(nextWord);}	//All punct word
		else {delimitPush(nextWord);}							//Annoying word :(
    }
	return true;
}

//Read file2
bool Document::ReadFile2 (istream& istr){
	//While the input stream is working
	while (!istr.fail()){
		string nextWord;
		istr >> nextWord;
		//If it fails to read next word, check for eof
		if (istr.fail()){
			if (istr.eof())	{//If end of file...
				if (words.size() == 0){return false;} //No words
				return true;} 	 //Legal eof()
			else {return false;} //Illegal input, false
		}
		
		//Iterate through the word character by character, separating into multiple strings based on punc rules
		words.push_back(nextWord);
	}
	return true;
}

//ReadExceptions
bool Document::ReadExceptions (istream& istr){
	//While the input stream is working
	string line;
	while (getline(istr, line)){ //While there is still a line to get
		if (line.length() <= 2){continue;} // Empty line, just go to next loop :)
		istringstream iss(line);
		string word1;	//Word we will replace
		string word2;	//Word which replaces
		string extra;	//Hopefully this is empty!
		iss  >> std:: skipws >> word1 >> word2 >> extra;
		if (word1.length() == 0 || word2.length() == 0 || extra.length() != 0){
			return false;
		}
 		exceptions.push_back(word1);
		exceptions.push_back(word2); 
    }
	return true;
}

//ReadDoc Helper method 
bool Document::delimitPush(string& word){
	int start = 0;
	char prev;
	
	//New word!
    for (int i = 0; i < int(word.length()) ; i++){
		//cout << prev << endl;
		if (i == 0 && (i+1 < int(word.length()))){ // the " .02" case
			if ((word.at(0) == '.') && (isdigit(word.at(1)))){
				prev = 'a';
				i = 1;
			}
		}
		if (start == i){
			if (i == int(word.length())-1){words.push_back(word.substr(start, 1));} //One char word
			else{prev = word.at(i);} //Previous character, good for referencing
		} 
		
		//Alpha string start
		else if (isAlpha(prev)){
            if (!isAlpha(word.at(i))){ //punctuation after alpha case
				if((isdigit(prev)) && (i+1 < int(word.length())) && (word.at(i) == ',' || word.at(i) == '.')){ //Checking comma & period case (2,0 || 2.0)
					if (!isdigit(word.at(i+1))){
						words.push_back(word.substr(start, i-start));
						start = i;
						i--;
					}else{}//Don't break it up. If we enter this if then we have a statement like 2,0 -> treat as alphanumeric
				}
				else{
					words.push_back(word.substr(start, i-start));
					start = i;
					i--;
				}
			}
			else if (i == int(word.length())-1){
				words.push_back(word.substr(start, int(word.length() - start))); //Legal string hits end of 'word'
			}
			else{prev = word.at(i);}
		}
		//Punctuation string start
		else { 
			if (isAlpha(word.at(i))){
				words.push_back(word.substr(start, i-start));
				start = i;
				i--;
			}
			else if (i == int(word.length())-1){
				words.push_back(word.substr(start, int(word.length() - start))); //Legal string hits end of 'word'
			}
		}
	}
	return true;
}

//MarkAmbiguous method
bool Document::MarkAmbiguousCapitalization (){
	numSentences = 0;
 	bool sos = true; //First word of the document is start of sentence
	for (int i = 0 ; i < int(words.size()) ; i ++){
	//If sos, alphanumeric starting with capital, isn't an acronym and contains no digits
		if (sos){
			if (isAlpha(words[i]) && startsWithCapital(words[i]) && !isAcro(words[i])
			&&  !containsDigits(words[i])){
			//mark with ambiguous "+" at front
			words[i].insert(0, "+");
			}
			sos = false;
		}
		//If the string contains ".", "?", "!" then it is end of sentence, next word is sos
		if (isEOS(words[i])){
			sos = true;
			numSentences++;
		}
	}
	return true;
}
bool Document::FixAmbiguousCapitalization (){
	for (int i = 0 ; i < int(words.size()) ; i ++){
		//For every word that is ambiguously capitalized we will look through the 
		//rest of the words in the document. 
		if (words[i].front() == '+' && words[i].length() > 1){ //if the word starts with + but is more than just +
			if (isAlpha(words[i].at(1))){ //if the string is alpha after the plus
				int j = 0;
				string noPlus = words[i].substr(1,words[i].length()); //trim the + for comparing
				for ( ; j < int(words.size()) ; j++){
					//If the same word appears unambiguously capitalized in the same document, 
					//the word should be capitalized and remove the “+” while keeping the first letter capitalized. 
					if(noPlus == words[j]){
						words[i] = noPlus;	//Take that plus off
						break;} 
					/*Otherwise, if the word does not appear unambiguously capitalized within the same document, 
					we remove the “+” and convert the first letter to lowercase.*/
				}
				if (j == int(words.size())){
					noPlus[0] = tolower(noPlus[0]);
					words[i] = noPlus;
					}
			}
		}
	}
	 
	 
	return true;
}

//Coleman-Liau Index Calculator
double Document::CL(Document& doc) const{
	double a = countLetters(doc);
	double b = countWords(doc);
	double c = numSentences;
	//cout << "CountLetters: " << a << endl;
	//cout << "CountWords: " << b << endl;
	//cout << "CL: numSentences: " << c << endl;
	if (b == 0){
		return -999;
	}else{
		double L = ((a/b)*100);
		double S = ((c/b)*100);
		return ((0.0588*L) - (0.296*S) - (15.8));
		}
	return true;
}

//CL Helper methods
int Document::countLetters(Document& doc) const{
	int count = 0;
	for (int i = 0 ; i < int(doc.words.size()) ; i++){
		if (isAlpha(doc.words[i])){
			count += doc.words[i].length();
		}
	}
	return count;
}
int Document::countWords(Document& doc) const{
	int count = 0;
	for (int i = 0 ; i < int(doc.words.size()) ; i++){
		if (isAlpha(doc.words[i])){
			count++;
		}
	}
	return count;
}

//AmbiguousCapitalization Helper Methods
bool Document::isAlpha(string& str) const{
	//Look at every char
	for (int i = 0 ; i < int(str.length()) ; i++){
		//Is it alphanumeric?
		if(!isAlpha(str.at(i))){return false;}
	}
	return true;
}
bool Document::isAlpha(char a) const{
	//Is it alphanumeric?
	if((a >= 'A' && a <= 'Z') //Upper alpha?
	|| (a >= 'a' && a <= 'z') //Lower alpha?
	|| (a >= '0' && a <= '9') //Digit?
	|| (a == '\'')){		  //Apostrophe?
		return true;
	}
	return false;
}
bool Document::isPunct(string& str) const{
	//Look at every char
	for (int i = 0 ; i < int(str.length()) ; i++){
		//Is it punctuation?
		if(!isPunct(str.at(i))){return false;}
	}
	return true;
}
bool Document::isPunct(char a) const{
	if ((a >= '!' && a <= '&')
		|| (a >= '(' && a <= '/')
		|| (a >= ':' && a <= '@')
		|| (a >= '[' && a <= '`')
		|| (a >= '{' && a <= '~')){
			return true;}
	return false;
}
bool Document::isAcro(string& str) const{
	for (int i = 1 ; i < int(str.length()) ; i++){
		if(str.at(i) >= 'A' && str.at(i) <= 'Z'){
			return true;
		}
	}
	return false;
}
bool Document::isEOS(string& str) const{
	if (!isAlpha(str)){ //Only check non-alphanumeric strings
		for (int i = 0 ; i < int(str.length()) ; i++){
			//Does it contain "?", ".", or "!" ?
			if(str.at(i) == '.' || str.at(i) == '?' || str.at(i) == '!'){
				return true;
			}
		}
	}
	return false;
}
bool Document::containsDigits(string& str) const{
	if (isAlpha(str)){ //Only check strings of only alphanumeric
		for (int i = 0 ; i < int(str.length()) ; i++){
			//Does it contain a digit? 
			if(str.at(i) >= '0' && str.at(i) <= '9'){
				return true;
			}
		}
	}	
	return false;
}
bool Document::containsCapitals(string& str) const{
	if(isAlpha(str)){
		for (int i = 0 ; i < int(str.length()) ; i++){
			//Is the letter capital?
			if(str.at(i) >= 'A' && str.at(i) <= 'Z'){
				return true;
			}
		}
	}
	return false;
}
int Document::inExceptions(string& str) const{
	//cout << "inExceptions(" << str << ")" << endl;
	int index = -1;
	for (int i = 0 ; i < int(exceptions.size()) ; i=i+2){
		if (str == exceptions[i]){
			index = i+1;
			break;}
	}
	//cout << "index = " << index << endl;
	return index;
}


/*Stem is going to go through every word in words and stem the appropriate ones. 
	For each word that should be stemmed, we will compare it to each word that has index%2 = 0 and
	if we find one which matches, replace it with word with index+1.
	Otherwise make stemmer object and stem accordingly
	*/
bool Document::Stem(){
	//First we need to see if we NEED to stem the word
	for (int i = 0 ; i < int(words.size()) ; i++){
		if (isAlpha(words[i]) && words[i].length() > 2 && 
			!containsCapitals(words[i]) && !containsDigits(words[i])){
			//If it is not punctuation, longer than 2 letters, no caps and no digs, 
			if (inExceptions(words[i]) == -1){ //AND isn't in exceptions, stem it!
				Stemmer s1(&words[i]); 	//Stemmer object holds reference to words[i], will side effect
				s1.porter(); 			//Stem with the porter algorithm
			}
			else{ //Must be in exceptions, and stemmable. Do the replace thang
				words[i] = exceptions[inExceptions(words[i])];
				}
			}
	}
	return true;
}

//Sort
bool Document::Sort (){
	//Sort the words
	sort(words.begin(), words.end());
	
	//This will be where the error handling will hang out, just a placeholder for now :)
	if (words.size() == 0){return false;}
	
	return true;
}

///TODO: CHANGE TO MAP STRATEGY! MAKE THIS METHOD OUTDATED!
//Shrink the vector and populate count vector 
bool Document::ShrinkCount(){
	string current = "";
	for (int i = 0 ; i < int(words.size()) ; i++){
		if (current == words[i]){		//Found duplicate word
			i = i - 1;					//don't move i
			counts[i] = counts[i]+1; 	//Increment count
			words.erase(words.begin()+i);//erase duplicate word
		}
		else{ //If new word, add a new counter=1
			current = words[i];
			counts.push_back(1);
		}
	}
	return true;
}

//Kill all punctuation strings (PA6)
bool Document::TrimPunct(){
	for (int i = 0 ; i < int(words.size()) ; i++){
		if (isPunct(words[i])){ //if we find a completely punct word
			words.erase(words.begin()+i); 	//erase it
			i = i - 1;			//don't move i
		}
	}
	return true;
}

//Write in required format
bool Document::WriteDoc (ostream& ostr) const{
	//Make sure I can write
	if (ostr.fail()) return false;
	for (int i = 0 ; i < int(words.size()) ; i++){
		ostr << words[i] << " ";
		//ostr << counts[i];
		ostr << endl;
	}
	return true;
}

//Testing method very useful!
bool Document::WriteExcep (ostream& ostr) const{
	//Make sure I can write
	if (ostr.fail()) return false;
	
	//Do the write
	for (int i = 0 ; i < int(exceptions.size()) ; i++){
		string word1 = exceptions[i];
		i++;
		string word2 = exceptions[i];
		ostr << word1 << " -> " << word2 << endl;
	}
	return true;
}




















