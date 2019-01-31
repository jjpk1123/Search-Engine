#include<Stemmer.h>

//Implement Stemmer.h
	bool Stemmer::containsVowel(string str) const{
		for (int i = 0 ; i < int(str.length()) ; i++){
			if (isVowel(str, i)){return true;}
		}
		return false;
	}
	bool Stemmer::isVowel(string& str, int i) const{	
		//Is it an obvious vowel?
		//cout << "Looking at " << str.at(i) << " in " << str << endl;
		if(	str.at(i) == 'a' || str.at(i) == 'e' || 
			str.at(i) == 'i' || str.at(i) == 'o' || 
			str.at(i) == 'u'){
			return true;
		}
		else if (str.at(i) == 'y'){
			//Base case
			if (i == 0){return false;}
			else if (isVowel(str, i-1)){return false;}
			else {return true;}
		}
		return false;
	}
	bool Stemmer::suffixIs(string& str, string suffix) const{
		//Only check if suffix is shorter than or equal(?) length of string
		if (str.length() >= suffix.length()){
			if (str.substr(str.length() - suffix.length(), str.length()) == suffix){
				return true;
			}
		}
		return false;
	}
	string Stemmer::region1(string& str, string r1)const{
		/*Region1 is the substring that follows the first consonant (non-vowel) that follows a
		vowel. Region1 may be empty (it often is for short words). 
		Examples: Region1(try) is empty, but Region1(definition) is inition.*/
		//If there is an r1, we want to check for vowel using the whole word
		int offset = str.length() - r1.length(); 
		if (r1.length() == 0){offset = 0;} //If there is no r1
		for (int i = 0; i < int(str.length())-offset ; i++){
			if (isVowel(str, i+offset)){	//First vowel
				for (; i < int(str.length())-offset ; i++){
					if (!isVowel(str, i+offset)){ //First consonant after vowel
						return str.substr(i+1+offset, str.length()); //Letter right after consonant = i+1
					}
				}
			}
		}
		
		return "";
	}
	string Stemmer::region2(string& str)const{	
		/*Region2 is the Region1 of Region1. 
		In other words, Region2(definition) = Region1(inition) = ition*/
		string l = region1(str, "");
		return region1(str, l);
	}
	string Stemmer::preceder(string& str, string suffix)const{
		/*The preceder is the part of a word before a given suffix. For example, if the suffix is ing
		then the preceder of talking is talk.*/
		return str.substr(0, str.length() - suffix.length());
	}
	bool Stemmer::isDouble(string& str) const{
		/*A double is any of the following letter pairs: 
		{‘bb’, ‘dd’, ‘ff’, ‘gg’, ‘mm’, ‘nn’, ‘pp’, ‘rr’, 'tt’}.*/
		if (str == "bb" || str == "dd" || str == "ff" || 
			str == "gg" || str == "mm" || str == "nn" || 
			str == "pp" || str == "rr" || str == "tt"){
				return true;
			}
		return false;
	}
	bool Stemmer::endsInShortSyllable(string& str) const{
		//It ends in non-vowel, followed by vowel, followed by non-vowel != {w, x, y}
		if (str.length() > 2){
				if(!isVowel(str, int(str.length()-3)) &&
				isVowel(str, int(str.length()-2)) && !isVowel(str, int(str.length()-1)) &&
				str.at(str.length()-1) != 'w' && str.at(str.length()-1) != 'x' &&
				str.at(str.length()-1) != 'y'){
				return true;
			}
		}
		//or it is 2 characters long, and is a vowel following by non-vowel
		if (str.length() == 2 && isVowel(str, int(str.length()-2)) &&
			!isVowel(str, int(str.length())-1)){
				return true;
		}
		
		return false;
	}
	bool Stemmer::isShortWord(string& str) const{
		if (endsInShortSyllable(str) && region1(str, "") == ""){
			return true;}
		return false;
	}
	bool Stemmer::isValidLiEnding(char a) const{
		/*A valid li-ending is one of {‘c’, ‘d’, ‘e’, ‘g’, ‘h’, ‘k’, ‘m’, ‘n’, ‘r’, ‘t’}.*/
		if (a == 'c' || a == 'd' || a == 'e' || a == 'g' || a == 'h' || 
			a == 'k' || a == 'm' || a == 'n' || a == 'r' || a == 't'){
				return true;
			}
		return false;
	}
	bool Stemmer::replaceSuffix(string& str, string suffix, string replacement){
		//cout << "replacing " << suffix << " in " << str << " with " << replacement << endl;
		str = str.substr(0, str.length() - suffix.length()) + replacement;
		return true;
	}
	
	//Porter algorithm
	bool Stemmer::porter(){
	//	cout << "stemming word: " << *word << endl;
	//	cout << "step 1" << endl;
	 	step1();
	//	cout << "step 2" << endl;
		step2();
	//	cout << "step 3" << endl;
	 	step3();
	//	cout << "step 4" << endl;
	 	step4();
	//	cout << "step 5" << endl;
	 	step5();
	//	cout << "step 6" << endl;
	 	step6();
	//	cout << "step 7" << endl;
	 	step7();
	//	cout << "step 8" << endl;
	 	step8(); 
		return true;
	}
	bool Stemmer::step1(){
		//1.If the word begins with an apostrophe, remove the apostrophe.
		if (word->at(0) == '\''){*word = word->substr(1, word->length());}
		
		//2. Then remove the longest of these that exist in the string:
		if (suffixIs(*word, "'s'"))		{replaceSuffix(*word, "'s'", "");}	//'s'
		else if (suffixIs(*word, "'s"))	{replaceSuffix(*word, "'s", "");}	//'s
		else if (suffixIs(*word, "'"))	{replaceSuffix(*word, "'", "");}	//'
		return true;
		}		
	bool Stemmer::step2(){
		if (suffixIs(*word, "sses"))		{replaceSuffix(*word, "sses", "ss");} //sses
		else if (suffixIs(*word, "ied")){	//ied
			if(preceder(*word, "ied").length() > 1){replaceSuffix(*word, "ied", "i");}	
			else {replaceSuffix(*word, "ied", "ie");}
		}
		else if (suffixIs(*word, "ies")){	//ies
			if(preceder(*word, "ies").length() > 1){replaceSuffix(*word, "ies", "i");}
			else {replaceSuffix(*word, "ies", "ie");}
		}
		else if (suffixIs(*word, "us")){replaceSuffix(*word, "us", "us");} //us
		else if (suffixIs(*word, "ss")){replaceSuffix(*word, "ss", "ss");} //ss
		else if (suffixIs(*word, "s")){
			if  (containsVowel(word->substr(0, word->length()-2))){
			replaceSuffix(*word, "s", "");} //s
		}
		return true;
		}	
	bool Stemmer::step3(){
		string r1 = region1(*word, "");
		if (suffixIs(*word, "eedly")){
			if (suffixIs(r1, "eedly")){replaceSuffix(*word, "eedly", "ee");}
		}
 		else if (suffixIs(*word, "ingly")) {
			if (containsVowel(preceder(*word, "ingly"))){
				replaceSuffix(*word, "ingly", "");
				if (word->length() > 1){
					string end = word->substr(word->length()-2, word->length());
					if (end == "at" || end == "bl" || 
						end == "iz" || isShortWord(*word)){word->append("e");} 	//append an e
					else if (isDouble(end)){replaceSuffix(*word, "1", "");}		//this removes the last letter
					}
				}
		}
		else if (suffixIs(*word, "edly")) {
			if (containsVowel(preceder(*word, "edly"))){
				replaceSuffix(*word, "edly", "");
				if (word->length() > 1){
				string end = word->substr(word->length()-2, word->length());
				if (end == "at" || end == "bl" || 
					end == "iz" || isShortWord(*word)){word->append("e");} 	//append an e
				else if (isDouble(end)){replaceSuffix(*word, "1", "");}		//this removes the last letter
				}
			}
		}
		else if (suffixIs(*word, "ing")) {
			if (containsVowel(preceder(*word, "ing"))){
				replaceSuffix(*word, "ing", "");
				if (word->length() > 1){
				string end = word->substr(word->length()-2, word->length());
				if (end == "at" || end == "bl" || 
					end == "iz" || isShortWord(*word)){word->append("e");} 	//append an e
				else if (isDouble(end)){replaceSuffix(*word, "1", "");}		//this removes the last letter
				}
			}
		}
 		else if (suffixIs(*word, "eed")){
			if  (suffixIs(r1, "eed")){
			replaceSuffix(*word, "eed", "ee");
			}
		}
	
		else if (suffixIs(*word, "ed")) {
			if (containsVowel(preceder(*word, "ed"))){
				replaceSuffix(*word, "ed", "");
				if (word->length() > 1){
				string end = word->substr(word->length()-2, word->length());
				if (end == "at" || end == "bl" || 
					end == "iz" || isShortWord(*word)){
						word->append("e");} 	//append an e
				else if (isDouble(end)){replaceSuffix(*word, "1", "");}		//this removes the last letter
				}
			}  
		}			
 		return true;
		}	
	bool Stemmer::step4(){
		if (suffixIs(*word, "y")){
			string pre = preceder(*word, "y");
			if (pre.length() > 1){
				if (!isVowel(pre, pre.length()-1) && pre.length() > 1){replaceSuffix(*word, "y", "i");}
			}
		}
		return true;
		}	
	bool Stemmer::step5(){
		//7 Letter suffixes
		if 		(suffixIs(*word, "ization"))	{replaceSuffix(*word, "ization", "ize");}
		else if (suffixIs(*word, "ational"))	{replaceSuffix(*word, "ational", "ate");}
		else if (suffixIs(*word, "fulness"))	{replaceSuffix(*word, "fulness", "ful");}
		else if (suffixIs(*word, "ousness"))	{replaceSuffix(*word, "ousness", "ous");}
		else if (suffixIs(*word, "iveness"))	{replaceSuffix(*word, "iveness", "ive");}
		
		//6 Letter suffixes
		else if (suffixIs(*word, "tional"))	{replaceSuffix(*word, "tional", "tion");}
		else if (suffixIs(*word, "iviti"))	{replaceSuffix(*word, "iviti", "ive");}
		else if (suffixIs(*word, "biliti"))	{replaceSuffix(*word, "biliti", "ble");}
		else if (suffixIs(*word, "lessli"))	{replaceSuffix(*word, "lessli", "less");}
		
		//5 Letter suffixes
		else if (suffixIs(*word, "entli"))	{replaceSuffix(*word, "entli", "ent");}
		else if (suffixIs(*word, "ation"))	{replaceSuffix(*word, "ation", "ate");}
		else if (suffixIs(*word, "alism"))	{replaceSuffix(*word, "alism", "al");}
		else if (suffixIs(*word, "aliti"))	{replaceSuffix(*word, "aliti", "al");}
		else if (suffixIs(*word, "ousli"))	{replaceSuffix(*word, "ousli", "ous");}
		else if (suffixIs(*word, "fulli"))	{replaceSuffix(*word, "fulli", "ful");}
		
		//4 Letter suffixes
		else if (suffixIs(*word, "alli"))	{replaceSuffix(*word, "alli", "al");}
		else if (suffixIs(*word, "enci"))	{replaceSuffix(*word, "enci", "ence");}
		else if (suffixIs(*word, "anci"))	{replaceSuffix(*word, "anci", "ance");}
		else if (suffixIs(*word, "abli"))	{replaceSuffix(*word, "abli", "able");}
		else if (suffixIs(*word, "izer"))	{replaceSuffix(*word, "izer", "ize");}
		else if (suffixIs(*word, "ator"))	{replaceSuffix(*word, "ator", "ate");}
		
		//3 Letter suffixes
		else if (suffixIs(*word, "bli"))	{replaceSuffix(*word, "bli", "ble");}
		else if (suffixIs(*word, "ogi")){
			if(preceder(*word, "ogi").back() == 'l'){
				replaceSuffix(*word, "ogi", "og");
			}
		}
		//2 Letter suffix
		else if (suffixIs(*word, "li")){
			if(isValidLiEnding(preceder(*word, "li").back())){
				replaceSuffix(*word, "li", "");
			}
		}
		return true;
		}	
	bool Stemmer::step6(){
		string r1 = region1(*word, "");
		string r2 = region2(*word);
		//7 Letter suffix in r1
		if (suffixIs(*word, "ational")){
			if (suffixIs(r1, "ational"))		{replaceSuffix(*word, "ational", "ate");}
		}
		//6 Letter suffix in r1
		else if (suffixIs(*word, "tional")) {
			if (suffixIs(r1, "tional"))			{replaceSuffix(*word, "tional", "tion");}
		}
		//5 Letter suffix in r1 OR r2
		else if (suffixIs(*word, "iciti")) {
			if (suffixIs(r1, "iciti"))		{replaceSuffix(*word, "iciti", "ic");}
		}
		else if (suffixIs(*word, "alize")){ 
			if (suffixIs(r1, "alize"))		{replaceSuffix(*word, "alize", "al");}
		}
		else if (suffixIs(*word, "icate")) {
			if (suffixIs(r1, "icate"))		{replaceSuffix(*word, "icate", "ic");}
		}
		else if (suffixIs(*word, "ative")) {
			if (suffixIs(r2, "ative"))		{replaceSuffix(*word, "ative", "");}
		}
		
		//Letter 4 suffix in r1
		else if (suffixIs(*word, "ical")) {
			if (suffixIs(r1, "ical"))		{replaceSuffix(*word, "ical", "ic");}
		}
		else if (suffixIs(*word, "ness")) {
			if (suffixIs(r1, "ness"))		{replaceSuffix(*word, "ness", "");}
		}
		
		//Letter 3 suffix in r1
		else if (suffixIs(*word, "ful")) {
			if (suffixIs(r1, "ful"))			{replaceSuffix(*word, "ful", "");}
		}
		return true;
		}	
	bool Stemmer::step7(){
		/*Format: if (the suffix is on the word){
					if((the suffix is in r2) && (maybe something else)){
						trim it off}
		*/
		
		string r2 = region2(*word);
		if 		(suffixIs(*word, "ement")) {
			if (suffixIs(r2, "ement"))	{replaceSuffix(*word, "ement", "");}
		}
		else if (suffixIs(*word, "ance")) {
			if (suffixIs(r2, "ance"))	{replaceSuffix(*word, "ance", "");}
		}
		else if (suffixIs(*word, "ence")) {
			if (suffixIs(r2, "ence"))	{replaceSuffix(*word, "ence", "");}
		}
		else if (suffixIs(*word, "able")) {
			if (suffixIs(r2, "able"))	{replaceSuffix(*word, "able", "");}
		}
		else if (suffixIs(*word, "ible")) {
			if (suffixIs(r2, "ible"))	{replaceSuffix(*word, "ible", "");}
		}
		else if (suffixIs(*word, "ment")) {
			if (suffixIs(r2, "ment"))	{replaceSuffix(*word, "ment", "");}
		}
		else if (suffixIs(*word, "ant")) {
			if (suffixIs(r2, "ant"))	{replaceSuffix(*word, "ant", "");}
		}
		else if (suffixIs(*word, "ent")) {
			if (suffixIs(r2, "ent"))	{replaceSuffix(*word, "ent", "");}
		}
		else if (suffixIs(*word, "ism")) {
			if (suffixIs(r2, "ism"))	{replaceSuffix(*word, "ism", "");}
		}
		else if (suffixIs(*word, "ate")) {
			if (suffixIs(r2, "ate"))	{replaceSuffix(*word, "ate", "");}
		}
		else if (suffixIs(*word, "iti")) {
			if (suffixIs(r2, "iti"))	{replaceSuffix(*word, "iti", "");}
		}
		else if (suffixIs(*word, "ous")) {
			if (suffixIs(r2, "ous"))	{replaceSuffix(*word, "ous", "");}
		}
		else if (suffixIs(*word, "ive")) {
			if (suffixIs(r2, "ive"))	{replaceSuffix(*word, "ive", "");}
		}
		else if (suffixIs(*word, "ize")) {
			if (suffixIs(r2, "ize"))	{replaceSuffix(*word, "ize", "");}
		}
		else if (suffixIs(*word, "ion")) {
			if (suffixIs(r2, "ion") && (preceder(*word, "ion").back() == 's'
									||	preceder(*word, "ion").back() == 't')) 					 
										{replaceSuffix(*word, "ion", "");}
		}
		else if (suffixIs(*word, "al")) {
			if (suffixIs(r2, "al"))	{replaceSuffix(*word, "al", "");}
		}
		else if (suffixIs(*word, "er")) {
			if (suffixIs(r2, "er"))	{replaceSuffix(*word, "er", "");}
		}
		else if (suffixIs(*word, "ic")) {
			if (suffixIs(r2, "ic"))	{replaceSuffix(*word, "ic", "");}
		}
		return true;
		}	
	bool Stemmer::step8(){
		string r1 = region1(*word, "");
		string r2 = region2(*word);
		string pre = preceder(*word, "e");
		if (suffixIs(*word, "e")){
			if (suffixIs(r2, "e") ||
			(suffixIs(r1, "e") && !endsInShortSyllable(pre)))
			{replaceSuffix(*word, "e", "");}
		}
		else if (suffixIs(*word, "l")){
			if (suffixIs(r2, "l") && preceder(*word, "l").back() == 'l')
			{replaceSuffix(*word, "l", "");}
		}
		return true;
		}
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
