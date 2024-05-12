#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ALPHABET_SIZE 26

struct Node{
	Node* children[26];
	char* definition;
	int isEndOfWordFlag;
};

Node* create_node(){
	Node* node = (Node*) malloc (sizeof(Node));
	for(int i = 0; i < ALPHABET_SIZE; i++){
		node->children[i] = NULL;
	}
	node->definition = NULL;
	node->isEndOfWordFlag = 0;
	
	return node;
}

int check_spaces(char* string){
	for(int i = 0; string[i] != '\0'; i++){
		if(string[i] == ' '){
			return 0;
		}
	}
	return 1;
}

void insert(Node* root, const char* word, const char* definition){
	int idx;
	Node* node = root;
	for(int i = 0; word[i] != '\0'; i++){
		idx = word[i] - 'a';
		if(node->children[idx] == NULL){
			node->children[idx] = create_node();
		}
		node = node->children[idx];
	}
	// Checks if word already exists
	// Then updates definition
	if(node->isEndOfWordFlag){
		printf("Succesfully updated a slang word\n");
		free(node->definition);
	} else{
		printf("Succesfully released new slang word\n");
	}
	printf("lol");
	node->isEndOfWordFlag = 1;
	node->definition = strdup(definition);
	printf("Nah\n");
}

void insert_menu(Node* root){
	char word[200];
	char definition[200];
	
	while(true){
		printf("Input a new slang word [Must be more than 1 characters and contains no space]: ");
		scanf("%s", word);
		getchar();
		int word_length = strlen(word);
		// Checks if there's more than 1 word or spaces
		if(word_length > 1 && check_spaces(word) == 1){
			break;
		}
		printf("\n");
	}
	
	while(true){
		printf("Input a new slang word description [Must be more than 2 words]: ");
		scanf("%[^\n]", definition);
		getchar();
		int def_length = strlen(definition);
		if(def_length > 2){
			break;
		}
		printf("\n");
	}
	
	insert(root, word, definition);
	printf("Press Enter to continue...\n");
	getchar();
}                               


void search(Node* root, const char* word){
	Node* node = root;
	int flag = 1;
	int idx;
	for(int i = 0; word[i] != '\0'; i++){
		idx = word[i] - 'a';
		if(node->children[idx] == NULL){
			flag = 0;
			break;
		}
		node = node->children[idx];
	}
	
	if(node != NULL && node->isEndOfWordFlag == 1 && flag == 1){
		printf("Slang Word: %s\n", word);
		printf("Definition: %s\n", node->definition);
	} else{
		printf("There is no word %s in the dictionary.\n", word);
	}
}

void search_menu(Node* root){
	char word[200];
	
	while(true){
		printf("Input a slang word to be searched [Must be more than 1 characters and contains no space]: ");
		scanf("%s", word);
		getchar();
		int word_length = strlen(word);
		// Check if more than one chars and no spaces
		if(word_length > 1 && check_spaces(word)){
			break;
		}
		printf("\n");
	}
	search(root, word);
	printf("Press enter to continue...\n");
	getchar();
}

void display_all_prefix_recur(Node* root, char* words, int depth, int* count);

void display_all_prefix_recur(Node* root, char* words, int depth, int* count){
	if(root == NULL){
		return;
	}
	if(root->isEndOfWordFlag){
		words[depth] = '\0';
		printf("%d. %s\n", ++(*count), words);
	}
	for(int i = 0; i < ALPHABET_SIZE; i++){
		if(root->children[i]){
			words[depth] = i + 'a';
			display_all_prefix_recur(root->children[i], words, depth+1, count);
		}
	}
}

void display_all_prefix(Node* root, const char* prefix){
	Node* node = root;
	int idx;
	int length = strlen(prefix);
	int count = 0;
	char words[200];
	strcpy(words, prefix);
	for(int i = 0; prefix[i] != '\0'; i++){
		idx = prefix[i] - 'a';
		if(node->children[idx] == NULL){
			printf("There is no prefix %s in the dictionary.\n", prefix);
		}
		node = node->children[idx];
	}

	printf("Words starts with %s:\n", words);
	display_all_prefix_recur(node, words, length, &count);
}

void display_all_words(Node* root, char* words, int depth, int *count);

void display_all_words(Node* root, char* words, int depth, int *count){
	if(root == NULL){
		return;
	}
	if(root->isEndOfWordFlag){
		words[depth] = '\0';
		printf("%d. %s\n", ++(*count), words);
	}
	for(int i = 0; i < ALPHABET_SIZE; i++){
		if(root->children[i]){
			words[depth] = i + 'a';
			display_all_words(root->children[i], words, depth+1, count);
		}
	}
}

void display_all_words_menu(Node* root){
	int flag = 0;
	for(int i = 0; i < ALPHABET_SIZE; i++){
		if(root->children[i]){
			flag = 1;
			break;
		}
	}
	
	if(flag == 0){
		printf("There is no slang word yet in the dictionary.\n");
	} else{
		char words[200];
		int count = 0;
		display_all_words(root, words, 0, &count);
	}
}

void display_all_prefix_menu(Node* root){
	char inp[200];
	printf("Input a prefix to be searched: ");
	scanf("%s", inp);
	getchar();
	printf("\n");
	display_all_prefix(root, inp);
}

void menu(){
	int input;
	Node* root = create_node();

	while(true){
		printf("1. Release a new slang word\n");
		printf("2. Search a slang word\n");	
		printf("3. View all slang words starting with a certain prefix word\n");
		printf("4. View all slang words\n");
		printf("5. Exit\n");
		printf("Insert number to input: ");
		scanf("%d", &input);
		getchar();
		
		
		switch(input){
			case 1:
				insert_menu(root);
				break;
			case 2:
				search_menu(root);
				break;
			case 3:
				display_all_prefix_menu(root);
				break;
			case 4:
				display_all_words_menu(root);
				break;
			case 5:
				printf("Thank you have a nice day\n");
				getchar();
				return;
				break;
		}
		printf("\n");
	}
}

int main(){
	menu();
	return 0;
}