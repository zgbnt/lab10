#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Trie structure
struct Trie
{	
	char character;
	struct Trie* nextchar[26];
	int occurrences;
};

// Initializes a trie structure
struct Trie *createTrie()
{
	struct Trie* newTrie = (struct Trie*)malloc(sizeof(struct Trie));
	for(int i = 0; i < 26; i++) {
		newTrie->character = 0;
		newTrie->nextchar[i] = NULL;
		newTrie->occurrences = 0;
	}
	return newTrie;
}

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word)
{
	if(pTrie == NULL) {
		return;
	}

	for(int i = 0; i < strlen(word); i++) {
		char c = word[i];
		if(pTrie->nextchar[c - 'a'] != NULL) {
			pTrie = pTrie->nextchar[c - 'a'];
		}
		else {
			pTrie->nextchar[c - 'a'] = createTrie();
			pTrie->nextchar[c - 'a']->character = c;
			pTrie = pTrie->nextchar[c - 'a'];
		}
	}
	pTrie->occurrences += 1;

	return;
}

// computes the number of occurances of the word
int numberOfOccurances(struct Trie *pTrie, char *word)
{
	if(pTrie == NULL) {
		return -1;
	}

	for(int i = 0; i < strlen(word); i++) {
		char c = word[i];
		if(pTrie->nextchar[c - 'a'] != NULL) {
			pTrie = pTrie->nextchar[c - 'a'];
		}
		else return 0;
	}
	return pTrie->occurrences;
}

// deallocate the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie)
{
	if(pTrie == NULL) {
		return pTrie;
	}

	for(int i = 0; i < 26; i++) {
		if(pTrie->nextchar[i] != NULL) {
			pTrie->nextchar[i] = deallocateTrie(pTrie->nextchar[i]);
		}
	}
	free(pTrie);
	pTrie = NULL;
	return pTrie;
}

// this function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords)
{
	FILE* fptr = fopen(filename, "r");
	if(fptr == NULL) printf("Could not open file\n");
	int numWords = 0;
	fscanf(fptr, "%d", &numWords);
	for(int i = 0; i < numWords; i++) {
		pInWords[i] = (char*)malloc(100 * sizeof(char));
		fscanf(fptr, "%s", pInWords[i]);
	}
	return numWords;
}

int main(void)
{
	char *inWords[256];
	
	//read the number of the words in the dictionary
	int numWords = readDictionary("dictionary.txt", inWords);
	for (int i=0;i<numWords;++i)
	{
		printf("%s\n",inWords[i]);
	}
	
	struct Trie *pTrie = createTrie();
	for (int i=0;i<numWords;i++)
	{
		insert(pTrie, inWords[i]);
	}
	// parse lineby line, and insert each word to the trie data structure
	char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
	for (int i=0;i<5;i++)
	{
		printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
	}
	for(int i = 0; i < numWords; i++) {
		free(inWords[i]);
	}
	pTrie = deallocateTrie(pTrie);
	if (pTrie != NULL)
		printf("There is an error in this program\n");
	return 0;
}