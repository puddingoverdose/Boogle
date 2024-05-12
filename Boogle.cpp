#include <stdio.h> // Standard input output library
#include <stdlib.h> // Melakukan memory allocation
#include <string.h> // Library untuk string
#define ALPHABET_SIZE 26 // Global variabel untuk menyimpan jumlah huruf dalam alfabet

// Node untuk struktur trie
struct Node{
	Node* children[26]; // Menyimpan 26 karakter huruf alfabet
	char* definition; // Menyimpan definisi kata-kata dalam trie
	int isEndOfWordFlag; // Menandakan apakah node sudah membentuk kata
};

/*
	Membuat Node baru yang dialokasikan memorinya
	Isi variabel dari node masih kosong (NULL)
	return Node baru
*/
Node* create_node(){
	Node* node = (Node*) malloc (sizeof(Node));
	
	// Karena node masih baru dibuat, 26 children dari node diinisialisasi NULL
	for(int i = 0; i < ALPHABET_SIZE; i++){
		node->children[i] = NULL;
	}
	node->definition = NULL; // Definisi masih belum ada maka NULL
	node->isEndOfWordFlag = 0; // Node belum diisi kata sehingga isEndOfWordFlag -> 0
	
	return node;
}

/*
	Mengecek apakah ada spasi dalam string input untuk fungsi
	insert_menu()
	return 1 (true) bila tidak ada spasi
	return 0 (false) bila ada spasi 
*/
int check_spaces(char* string){
	for(int i = 0; string[i] != '\0'; i++){
		if(string[i] == ' '){
			return 0;
		}
	}
	return 1;
}

/*
	Insert adalah fungsi untuk memasukkan node baru
*/
void insert(Node* root, const char* word, const char* definition){
	int idx;
	Node* node = root;
	
	// Untuk setiap kata dalam word
	// Akan dicek apabila sudah ada node huruf yang berisi pada trie
	// Jika tidak, akan dibuat node baru huruf tersebut
	// Lalu node akan ditraverse seterusnya pada trie
	for(int i = 0; word[i] != '\0'; i++){
		idx = word[i] - 'a';
		if(node->children[idx] == NULL){
			node->children[idx] = create_node();
		}
		node = node->children[idx];
	}
	
	// Jika kata sudah dalam dictionary
	// Definisi lama dihapus
	if(node->isEndOfWordFlag){
		printf("Succesfully updated a slang word\n");
		free(node->definition);
	} else{
		printf("Succesfully released new slang word\n");
	}
	node->isEndOfWordFlag = 1; // Kata sudah terbuat, maka update isEndOfWordFlag menjadi 1 (true)
	
	// strdup digunakan untuk mengcopy definition karena pada struct Node 
	// variabel char* definition menggunakan dynamic memory
	node->definition = strdup(definition);
}

/*
	Menu untuk fungsi insert()
*/
void insert_menu(Node* root){
	char word[200]; // Input kata
	char definition[200]; // Input definisi
	
	// Mengecek apakah input kata pengguna sudah benar
	while(true){
		printf("Input a new slang word [Must be more than 1 characters and contains no space]: ");
		scanf("%s", word);
		getchar();
		int word_length = strlen(word);
		// Mengecek jika panjang kata > 1 dan apakah mengandung spasi
		if(word_length > 1 && check_spaces(word) == 1){
			break;
		}
		printf("\n");
	}
	
		
	// Mengecek apakah input definisi pengguna sudah benar
	while(true){
		printf("Input a new slang word description [Must be more than 2 words]: ");
		scanf("%[^\n]", definition);
		getchar();
		int def_length = strlen(definition);
		// Mengecek jika panjang kata > 2
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
	// Memulai pencarian dari root
	// Variabel node akan mengtraverse root
	Node* node = root;
	int flag = 1; // Penanda adanya kata atau tidak
	int idx;
	
	// Jika ada huruf dalam word yang tidak ada pada children
	// node pada saat traversal root
	// Maka tidak ada kata dalam trie
	for(int i = 0; word[i] != '\0'; i++){
		idx = word[i] - 'a';
		if(node->children[idx] == NULL){
			flag = 0;
			break;
		}
		node = node->children[idx];
	}
	
	// Jika node tidak kosong, node adalah akhir dari suatu kata, dan flag = 1
	// Maka kata ada dalam trie
	if(node != NULL && node->isEndOfWordFlag == 1 && flag == 1){
		printf("Slang Word: %s\n", word);
		printf("Definition: %s\n", node->definition);
	} else{
		printf("There is no word %s in the dictionary.\n", word);
	}
}

/*
	Fungsi menu pada search
*/
void search_menu(Node* root){
	char word[200]; // Input kata yang mau dicari
	
	while(true){
		printf("Input a slang word to be searched [Must be more than 1 characters and contains no space]: ");
		scanf("%s", word);
		getchar();
		int word_length = strlen(word);
		// Mengecek jika panjang kata > 1 dan apakah ada spasi
		if(word_length > 1 && check_spaces(word)){
			break;
		}
		printf("\n");
	}
	
	// Menjalankan fungsi search()
	search(root, word);
	printf("Press enter to continue...\n");
	getchar();
}

// Untuk rekursi fungsi display_all_prefix_recur
void display_all_prefix_recur(Node* root, char* words, int depth, int* count);

/*
	Fungsi rekursif untuk mencari prefix pada kata
	Rekursif digunakan untuk mengecek apa saja kata yang memiliki prefix dimaksud
*/
void display_all_prefix_recur(Node* root, char* words, int depth, int* count){
	// Jika root sudah kosong, maka tidak ada kata setelah kata ini yang memiliki prefix
	if(root == NULL){
		return;
	}
	
	// Jika root->isEndOfWordFlag = 1, maka ada kata yang memiliki prefix 
	// Print kata tersebut
	if(root->isEndOfWordFlag){
		// words[depth] = '\0'; berarti menutup string dimana kata dengan prefix sudah ditemukan
		
		// ++(*count) berarti pada variabel pointer int* count
		// Nilai variabel yang dimiliki oleh variabel yang 
		// direferensi oleh pointer di pre-increment tambah 1
		// Hal ini untuk menjadikan urutan angka saat printf() dijalankan
		// terjaga
		words[depth] = '\0';
		printf("%d. %s\n", ++(*count), words);
	}
	
	// Untuk setiap huruf pada alfabet
	// Jika root memiliki children pada index i
	// Maka pada words akan ditambahkan huruf
	// dengan nilai ascii i + 'a'
	// yang merupakan kata pada trie yang memiliki prefix dimaksud
	
	// Dijalankan display_all_words() rekursif untuk mencari kata-kata dengan prefix lain
	// Dilakukan traversal pada node root dengann children i tersisa pada trie
	// Depth+1 untuk mencari pada kedalaman +1 lebih dalam pada trie
	for(int i = 0; i < ALPHABET_SIZE; i++){
		if(root->children[i]){
			words[depth] = i + 'a';
			display_all_prefix_recur(root->children[i], words, depth+1, count);
		}
	}
}

/*
	Fungsi untuk display semua kata yang memiliki prefix yang dicari
*/
void display_all_prefix(Node* root, const char* prefix){
	// Traversal dimulai dari root
	Node* node = root;
	int idx;
	int depth = strlen(prefix); // Menampung panjang dari prefix, dan juga merupakan kedalaman prefix pada trie
	int count = 0;
	char words[200]; // Untuk menampung kata-kata yang memiliki prefix dicari
	strcpy(words, prefix); // Words menampung prefix terlebih dahulu
	
	// Memastikan apakah prefix ada
	// Pada saat traversal root trie
	for(int i = 0; prefix[i] != '\0'; i++){
		idx = prefix[i] - 'a';
		if(node->children[idx] == NULL){
			printf("There is no prefix %s in the dictionary.\n", prefix);
		}
		// Update node yang menjadi referensi untuk 
		// Pencarian kata-kata dengan prefix
		node = node->children[idx];
	}

	printf("Words starts with %s:\n", words);
	display_all_prefix_recur(node, words, depth, &count);
}

/*
	Fungsi rekursif display_all_words()
*/
void display_all_words(Node* root, char* words, int depth, int *count);

/*
	Fungsi rekursif display_all_words()
*/
void display_all_words(Node* root, char* words, int depth, int *count){
	// Jika root sudah kosong, maka tidak ada kata setelah kata ini yang memiliki prefix
	if(root == NULL){
		return;
	}
	
	// Jika root->isEndOfWordFlag = 1, maka ada kata yang memiliki prefix 
	// Print kata tersebut
	if(root->isEndOfWordFlag){
		// words[depth] = '\0'; berarti menutup string dimana kata dengan prefix sudah ditemukan
		
		// ++(*count) berarti pada variabel pointer int* count
		// Nilai variabel yang dimiliki oleh variabel yang 
		// direferensi oleh pointer di pre-increment tambah 1
		// Hal ini untuk menjadikan urutan angka saat printf() dijalankan
		// terjaga
		words[depth] = '\0';
		printf("%d. %s\n", ++(*count), words);
	}
	
	// Untuk setiap huruf pada alfabet
	// Jika root memiliki children pada index i
	// Maka pada words akan ditambahkan huruf
	// dengan nilai ascii i + 'a'
	// yang merupakan kata pada trie yang memiliki prefix dimaksud
	
	// Dijalankan display_all_words() rekursif untuk mencari kata-kata dengan prefix lain
	// Dilakukan traversal pada node root dengann children i tersisa pada trie
	// Depth+1 untuk mencari pada kedalaman +1 lebih dalam pada trie
	for(int i = 0; i < ALPHABET_SIZE; i++){
		if(root->children[i]){
			words[depth] = i + 'a';
			display_all_words(root->children[i], words, depth+1, count);
		}
	}
}

/*
	Menu untuk fungsi display_all_words()
*/
void display_all_words_menu(Node* root){
	int flag = 0;
	// Mengecek apakah ada node root memiliki setidaknya satu children huruf atau tidak
	for(int i = 0; i < ALPHABET_SIZE; i++){
		if(root->children[i]){
			flag = 1;
			break;
		}
	}
	
	if(flag == 0){
		printf("There is no slang word yet in the dictionary.\n");
	} else{
		char words[200]; // Menyimpan kata-kata pada trie
		int count = 0; // Untuk menjaga urutan kata pada trie
		display_all_words(root, words, 0, &count);
	}
}

/*
	Menu untuk fungsi display_all_prefix()
*/
void display_all_prefix_menu(Node* root){
	char inp[200]; //Input prefix yang mau dicari
	printf("Input a prefix to be searched: ");
	scanf("%s", inp);
	getchar();
	printf("\n");
	display_all_prefix(root, inp);
}

/*
	Fungsi menu
*/
void menu(){
	int input;
	Node* root = create_node(); //Inisialisasi root awal kosong untuk trie
	
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
