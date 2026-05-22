#include "AVLTree.h"

#include <iostream>


#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
using namespace std;



char convert_lowercase(char c){

    if (c >= 'A' && c <= 'Z'){
        return c + 32;
    }

    return c;

}



//actual class starting

Avltree::Avltree() :root(NULL){}

//destroy all nodes recursively
Avltree::~Avltree(){

    destroy_tree(root);
 }

//post order tree node deletion children first then parent
void Avltree::destroy_tree(MovieNode* node){

    if (!node){
       return;
     }

    destroy_tree(node->left);
    destroy_tree(node->right);
    delete node;

}

//height of node
int Avltree::getheight(MovieNode* node){

    if (node!=NULL){
        return node->height;
     }

    else{
        return 0;

    }

}

// returns balance lh - rh
int Avltree::getbalance(MovieNode* node){

    if (node!=NULL){
        return getheight(node->left) - getheight(node->right);
    }


    else{
        return 0;
    }

}

// compare string lexicographically 
int Avltree::compare_strings(string a, string b){

    for (int i = 0; i < a.length() && i < b.length(); i++){  //stops when the end of one string is reached

        char charA = convert_lowercase(a[i]);

        char charB = convert_lowercase(b[i]);

        if (charA != charB){
            return charA-charB;

         }
    }

    return a.length() - b.length();


}

// compare movies by titles using compare_strings and then the year
int Avltree::compare_movies(MovieNode* mov1, MovieNode* mov2){

    int title_compare;

    title_compare = compare_strings(mov1->title, mov2->title);

    if (title_compare != 0){   //titles don't match return
        return title_compare;
     }

    // titles match tu compare by year
    return mov1->year - mov2->year;

}


MovieNode* Avltree::rotate_right(MovieNode* y){

    MovieNode* x = y->left;
    MovieNode* B = x->right;
    x->right = y;
    y->left = B;
    
    if (getheight(y->left) > getheight(y->right)){

        y->height = 1 + getheight(y->left);
    }

    else{

        y->height = 1 + getheight(y->right);
    }
    
    if (getheight(x->left) > getheight(x->right)){
        x->height = 1 + getheight(x->left);
     }

    else {
         x->height = 1 + getheight(x->right);
    }
    

    return x;


}


MovieNode* Avltree::rotate_left(MovieNode* x){

    MovieNode* y = x->right;
    MovieNode* B = y->left;
    
    y->left = x;
    x->right = B;
    
    if (getheight(x->left) > getheight(x->right)){
        x->height = 1 + getheight(x->left);
    }

    else{
        x->height = 1 + getheight(x->right);
     }
    
    if (getheight(y->left) > getheight(y->right)){
        y->height = 1 + getheight(y->left);
    }

    else {
         y->height = 1 + getheight(y->right);
    }
    

    return y;


}

// insertion helper
MovieNode* Avltree::insert_helper(MovieNode* node, MovieNode* newMovie){
    int cmp ;
    int balance;

    if (!node){
        return newMovie;

    }
    
    cmp =compare_movies(newMovie, node);

    if (cmp < 0){
        node->left = insert_helper(node->left, newMovie);
    }
    else if (cmp > 0){
        node->right= insert_helper(node->right, newMovie);
     }

    else {
        return node;//duplicate 
    }
    
    //height update
    if (getheight(node->left) > getheight(node->right)){
        node->height = 1 + getheight(node->left);
    }

    else{
        node->height= 1 + getheight(node->right);
    }
    
    //get balance factor
    balance = getbalance(node);
    
    //LL
    if (balance >1 && compare_movies(newMovie,node->left) < 0){
        return rotate_right(node);

    }
    
    //RR
    if (balance < -1 &&compare_movies(newMovie, node->right) > 0){
        return rotate_left(node);
     }
    
    //LR

    if (balance > 1&& compare_movies(newMovie , node->left) > 0){
        node->left = rotate_left(node->left);
        return rotate_right(node);
     }
    
    //RL

    if (balance < -1&&  compare_movies(newMovie,node->right) < 0){
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }


    
    return node;

}


void Avltree::insert(MovieNode* movie){

    root= insert_helper(root,movie);
}


// seraching all occurences of the title 
MovieNode* Avltree::search_helper(MovieNode* node, string title, bool findAll, bool& found){
    
    if (!node){
        return NULL;  
    }

    // left subtree searching 

    MovieNode* leftResult = search_helper(node->left, title, findAll, found);
    
    if (leftResult){
        return leftResult;  
    }

    // check current

    if (compare_strings(node->title, title) == 0){

        node->display_all();  
        found = true;  
        
        
        if (!findAll){
            return node; 
        }
    }

    //  right subtree searching
    return search_helper(node->right, title, findAll, found);
}

MovieNode* Avltree::search(string title){
    bool found;
    found =false;
    MovieNode* result =search_helper(root,title, false, found);

    return result; 

}


void Avltree::search_ALL_bytitle(string title){
    bool found;

    cout << endl;
    cout <<YELLOW << "=== Searching for movies titled: \"" << title << "\" ==="<<RESET << endl;

    found = false;
    search_helper(root, title, true, found); //Set findAll to true takay we can find all occurrences
    
    if (!found){
        cout << RED << "NO movies have been found with that title  "<< RESET <<endl;

    }

}


//inorder printing helper in alphabetical order

void Avltree::inorder_helper(MovieNode* node){

    if (!node){
      return;
    }

    inorder_helper(node->left);
    cout << node->title << " (";

    if (node->year == 0){

        cout << "N/A";
    }

    else{
        cout << node->year;
     }


    cout << ")" << endl;

    inorder_helper(node->right);

}


void Avltree::inorder_display(){
    inorder_helper(root);
}

//counting total nodes
int Avltree::count_noeds(MovieNode* node){
    
    if (!node){
        return 0;
    }

    return 1 + count_noeds(node->left) + count_noeds(node->right);

}



//inorder traversal used to collect the nodes from the avl tree and stored into an array it will be in alphabetic order
void Avltree::collect_allmovies(MovieNode* node, MovieNode** array, int& index){
    
    if (!node){
        return;
    }
    
    collect_allmovies(node->left, array, index);
    array[index++] = node;

    collect_allmovies(node->right, array, index);

}

//searches movies with the searched year
void Avltree::searchyear_helper(MovieNode* node, int year){
    if (!node){
        
        return;
    }

    searchyear_helper(node->left, year);
    
    if (node->year== year){
        node->display_all();

     }

    searchyear_helper(node->right,year);

}


void Avltree::search_byyear(int year){
    cout << YELLOW <<endl << "=== Movies released in " << year <<" ===" << RESET <<endl;
    searchyear_helper(root, year);

}

//searches for movies that lie within the raating range

void Avltree::searchratingsrange_helper(MovieNode* node, double minRating, double maxRating){
    if (!node){
        return;
    }
    searchratingsrange_helper(node->left, minRating, maxRating);
    
    if (node->rating >= minRating && node->rating <= maxRating){
        node->display_all();
    }
    searchratingsrange_helper(node->right, minRating, maxRating);
}


void Avltree::search_byratingrange(double minRating, double maxRating){

    cout << YELLOW <<endl << "=== Movies with rating between " << minRating<< " and " << maxRating<< " ===" << RESET<< endl;
    
    searchratingsrange_helper(root, minRating, maxRating);

}

