#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

int OSTYPE = 2; // 1 for mac, 2 for win

struct doc {
	int docID;
	int numOfTimes;
	int lenOfDoc;
	char nameOfCat[20];
	int hasNextDoc;
	struct doc* nextDoc;
};
typedef struct doc* Doc;

struct term {
    char name[20];
    int numOfDoc;
	int hasNextTerm;
	int hasEcon;
	int hasHealth;
	int hasMagazin;
	struct term* nextTerm;
	struct doc *firstDoc;
};
typedef struct term* Term;

Term getTerm();
Doc getDoc(char[], int, int);
void printStopWords(Term);
void printDiscWords(Term);
void printTermAll(Term);
void printList(Term);
void printTerm(Term);
void printDocAll(Doc);
void printDocList(Doc);
void printDoc(Doc);
void readCat(Term, char[]);
void addWordToTermList(Term, char[], Doc);
void addNewTermToList(Term, char[], int, Doc);
int readLengthOfFile(FILE *);
void readFile(FILE *, Term, char[], int, int);
void addWordToExistingList(Doc, Doc);

main (void) {
	Term firstTerm = getTerm();
    readCat(firstTerm, "econ");
    readCat(firstTerm, "health");
    readCat(firstTerm, "magazin");
    // printList(firstTerm);
   	printStopWords(firstTerm);
   	printDiscWords(firstTerm);
}

Term getTerm() {
	Term _term = malloc(sizeof(struct term));
	strcpy(_term->name, "");
	_term->numOfDoc = 0;
	_term->hasNextTerm = 0;
	_term->hasEcon = 0;
	_term->hasHealth = 0;
	_term->hasMagazin = 0;
	return _term;
}

Doc getDoc(char nameOfCat[], int docID, int lenOfDoc) {
	Doc _doc = malloc(sizeof(struct doc));
	strcpy(_doc->nameOfCat, nameOfCat);
	_doc->docID = docID;
	_doc->lenOfDoc = lenOfDoc;
	_doc->numOfTimes = 1;
	_doc->hasNextDoc = 0;
	return _doc;
}

void printStopWords(Term t){
	Term _curTerm = t->nextTerm;
	char *terms[50];
	int i = 0;
	while(_curTerm->hasNextTerm){
		while(_curTerm->firstDoc->hasNextDoc){
			if(strcmp(_curTerm->firstDoc->nameOfCat, "econ") == 0) {
				_curTerm->hasEcon = 1;
			}
			else if(strcmp(_curTerm->firstDoc->nameOfCat, "health") == 0)	{
				_curTerm->hasHealth = 1; 
		    }
			else if(strcmp(_curTerm->firstDoc->nameOfCat, "magazin") == 0)	{
				_curTerm->hasMagazin = 1; 
			}
			_curTerm->firstDoc = _curTerm->firstDoc->nextDoc;
		}
		//for last doc
		if(!_curTerm->firstDoc->hasNextDoc){
			if(strcmp(_curTerm->firstDoc->nameOfCat, "econ") == 0) {
			_curTerm->hasEcon = 1;
			}
			else if(strcmp(_curTerm->firstDoc->nameOfCat, "health") == 0)	{
			_curTerm->hasHealth = 1; 
	   	   }
	    	else if(strcmp(_curTerm->firstDoc->nameOfCat, "magazin") == 0)	{
			_curTerm->hasMagazin = 1; 
			}
		}
		if(_curTerm->hasEcon == 1 && _curTerm->hasHealth == 1 && _curTerm->hasMagazin == 1){
			terms[i] = _curTerm->name;
			i++;
		}	
		_curTerm = _curTerm->nextTerm;
	}
	for(i = 0; i < 5; i++){
		printf("Term-%d: %s \n", i+1, terms[i]);
	}
}
			
void printDiscWords(Term t){
	Term _curTerm = t->nextTerm;
	char *econ[5], *health[5], *magazin[5];
	int i = 0;
	int j = 0;
	int k = 0;
	while(_curTerm->hasNextTerm){
		if(_curTerm->hasEcon == 1 && _curTerm->hasHealth == 0 && _curTerm->hasMagazin == 0 && i < 5){
			econ[i] = _curTerm->name;
			i++;
		}	
		if(_curTerm->hasEcon == 0 && _curTerm->hasHealth == 1 && _curTerm->hasMagazin == 0 && j < 5){
			health[j] = _curTerm->name;
			j++;
		}
		if(_curTerm->hasEcon == 0 && _curTerm->hasHealth == 0 && _curTerm->hasMagazin == 1 && k < 5){
			magazin[k] = _curTerm->name;
			k++;
		}
		_curTerm = _curTerm->nextTerm;
	}
	printf("Category ECON      Category HEALTH    Category MAGAZIN\n");
	for(i = 0; i < 5; i++){
		printf("Term-%d : %8s  Term-%d : %8s  Term-%d : %8s\n", i+1, econ[i], i+1, health[i], i+1, magazin[i]);
	}
}

void printTermAll(Term t) {
	printf("\n --- term start --- \n");
	printf("Address: %p\n", (void*)t);
	printf("name %s \n", t->name);
	printf("hasNextTerm: %d \n", t->hasNextTerm);
	printf("numOfDoc: %d \n", t->numOfDoc);
	printf("Next Address: %p\n", (void*)(t->nextTerm));
	printf("\n --- term end --- \n");
}

void printDocAll(Doc d) {
	printf("\n --- doc start --- \n");
	printf("Address: %p\n", (void*)d);
	printf("nameOfCat %s \n", d->nameOfCat);
	printf("hasNextDoc: %d \n", d->hasNextDoc);
	printf("lenOfDoc: %d \n", d->lenOfDoc);
	printf("Next Address: %p\n", (void*)(d->nextDoc));
	printf("\n --- doc end --- \n");
}

void printList(Term curTerm) {
	printf("RESULTS: \n");
	// printTerm(curTerm);
	while (curTerm->hasNextTerm) {
		curTerm = curTerm->nextTerm;
		printTerm(curTerm);
	}
}

void printDocList(Doc curDoc) {
	printf("Docs in term: \n");
	printDoc(curDoc);
	while (curDoc->hasNextDoc) {
		curDoc = curDoc->nextDoc;
		printDoc(curDoc);
	}
}

void printTerm(Term t) {
	printf("%s\n", t->name);
	printDocList(t->firstDoc);
}

void printDoc(Doc d) {
	printf("\t%s-%d %d/%d\n", d->nameOfCat, d->docID, d->numOfTimes, d->lenOfDoc);
}

void readCat (Term firstTerm, char nameOfCat[]) {
	char _fileBaseName[80];
	char _fileName[100];
	strcpy(_fileBaseName, ".\\dataset\\");
	strcat(_fileBaseName, nameOfCat);
	FILE *inpf;
	int i = 1;
	sprintf(_fileName, "%s\\%d.txt", _fileBaseName, i);
	if (OSTYPE == 1) {
		printf("working on MAC\n");
		strcpy(_fileBaseName, "dataset/");
		strcat(_fileBaseName, nameOfCat);
		sprintf(_fileName, "%s/%d.txt", _fileBaseName, i);
	}
	printf("\n fileName %s\n", _fileName);
	while (inpf = fopen(_fileName, "r")) {
		printf("\n fileName %s\n", _fileName);
		int lenOfDoc = readLengthOfFile(inpf);
		inpf = fopen(_fileName, "r");
		readFile(inpf, firstTerm, nameOfCat, i, lenOfDoc);
		i = i + 1;
		sprintf(_fileName, "%s\\%d.txt", _fileBaseName, i);
		if (OSTYPE == 1) {
			sprintf(_fileName, "%s/%d.txt", _fileBaseName, i);
		}
	}
}

int readLengthOfFile(FILE *inpf) {
	int lenOfDoc = 0;
	while (!feof(inpf)) {
		char word[20];
		fscanf(inpf, "%s", word);
		lenOfDoc += 1;
	}
	fclose(inpf);
	return lenOfDoc;
}

void readFile(FILE *inpf, Term firstTerm, char nameOfCat[], int docID, int lenOfDoc) {
	while (!feof(inpf)) {
		char word[20];
		fscanf(inpf, "%s", word);
		Doc newDoc = getDoc(nameOfCat, docID, lenOfDoc);
		addWordToTermList(firstTerm, word, newDoc);
	}
	fclose(inpf);
}

void addWordToTermList(Term firstTerm, char word[], Doc newDoc) {
	Term _curTerm = firstTerm;
	Term _prevTerm = getTerm();
	while (strcmp(_curTerm->name , word) < 0) {
		if (!_curTerm->hasNextTerm) {
			addNewTermToList(_curTerm, word, 0, newDoc);
			return;
		}
		_prevTerm = _curTerm;
		_curTerm = _curTerm->nextTerm;
	}
	if (strcmp(_curTerm->name, word) == 0) {
		addWordToExistingList(_curTerm->firstDoc, newDoc);
	}
	else {
		addNewTermToList(_prevTerm, word, 1, newDoc);
	}
}

void addNewTermToList(Term prevTerm, char word[], int hasNextTerm, Doc newDoc) {
	Term newTerm = getTerm();
	strcpy(newTerm->name, word);
	newTerm->nextTerm = prevTerm->nextTerm;
	newTerm->hasNextTerm = hasNextTerm;
	newTerm->firstDoc = newDoc;
	prevTerm->nextTerm = newTerm;
	prevTerm->hasNextTerm = 1;
}

void addWordToExistingList(Doc firstDoc, Doc newDoc) {
	Doc _curDoc = firstDoc;
	while (_curDoc->hasNextDoc) {
		if (strcmp(_curDoc->nameOfCat, newDoc->nameOfCat) == 0 && _curDoc->docID == newDoc->docID) {
			_curDoc->numOfTimes += 1;
			return;
		}
		_curDoc = _curDoc->nextDoc;
	}
	if (strcmp(_curDoc->nameOfCat, newDoc->nameOfCat) == 0 && _curDoc->docID == newDoc->docID) {
			_curDoc->numOfTimes += 1;
	}
	else {
		_curDoc->nextDoc = newDoc;
		_curDoc->hasNextDoc = 1;
	}
}
