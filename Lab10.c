#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Trie structure
struct Trie
{
    int count;
    struct Trie *children[26];	
};

// Initializes a trie structure
struct Trie *createTrie()
{
    //i moved this to the top so i dont have to
    // rewrite the same code for the insert function

    //create the memory space
    struct Trie* temp = malloc(sizeof(struct Trie));
    
    //set all members to 0/NULL
    temp->count = 0;
    for (int i = 0; i < 26; i++)
    {
        temp->children[i] = NULL;
    }

    //return the generated memory space
    return temp;
}

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word)
{
    //traverse until final letter in the word 
    int i = 0;
    do
    {
        //if the next letter's node does not exist,
        // make a new one
        if (pTrie->children[word[i]-'a'] == NULL)
        {
            //create new node and attach it to parent
            pTrie->children[word[i]-'a'] = createTrie(pTrie);
        }
        
        //traverse
        pTrie = pTrie->children[word[i]-'a'];

        //iterate position counter
        //if the next letter to be traversed to is the
        // null terminator, exit the loop
        i++;
    } while (word[i] != '\0');
    
    //final letter reached, iterate count
    pTrie->count++;

}

// computes the number of occurances of the word
int numberOfOccurances(struct Trie *pTrie, char *word)
{
    //flag for words that do not exist
    int flag = 0;

    //traverse until final letter in the word 
    int i = 0;
    do
    {
        //if the word is not in the dictionary, abort
        if (pTrie->children[word[i]-'a'] == NULL)
        {
            flag = 1;
            break;
        }

        //traverse
        pTrie = pTrie->children[word[i]-'a'];

        //iterate position counter
        //if the next letter to be traversed to is
        // the null terminator, exit the loop
        i++;
    } while (word[i] != '\0');
    
    //if the flag was raised, return 0
    if (flag)
    {
        return 0;
    }

    //final letter reached, return count
    return pTrie->count;
}

// deallocate the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie)
{
    //recursively go down every existing tree
    for (int i = 0; i < 26; i++)
    {
        if (pTrie->children[i] != NULL)
        {
            deallocateTrie(pTrie->children[i]);
        }
    }

    //free the node
    free(pTrie);

    return NULL;
}

// this function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords)
{
    //open file
    FILE *fp = fopen("dictionary.txt", "r");
    
    //store number of words in the file
    int temp;
    fscanf(fp, "%d", &temp);

    //temporary string storage
    char wordT[100];

    //store each word in the given 2d char array
    for (int i = 0; i < temp; i++)
    {
        fscanf(fp, "%s", wordT);
        pInWords[i] = malloc(100 * sizeof(char));
        // pInWords[i] = (char*) calloc(strlen(wordT), sizeof(char));
        strcpy(pInWords[i], wordT);
    }

    //close the file
    fclose(fp);

    //return the number of words
    return temp;
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
	pTrie = deallocateTrie(pTrie);
	if (pTrie != NULL)
		printf("There is an error in this program\n");
	return 0;
}