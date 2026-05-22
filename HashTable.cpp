#include "HashTable.h"
#include "CSVParser.h"

#include <iostream>
using namespace std;


// cleans up movie list

ActorNode::~ActorNode(){

    while (moviesList){
        MovieListNode* temp = moviesList;
        moviesList = moviesList->next;
        delete temp; }

}

// displays all movies of an actor
void ActorNode::displayFilmography(){
    int count ;
    cout << endl;
    cout << "--- Filmography of " << actorName << " ---" << endl;

    MovieListNode* current = moviesList;
    count = 1;

    while (current){

        cout << count++ << ". " << current->movie->title << " (" << current->movie->year << ")" << endl;
        current = current->next;
     }

}

// adds a movie to an actors moviesList
void ActorNode::add_movie(MovieNode* movie){

    //duplicates
    MovieListNode* current = moviesList;

    while (current){

        if (current->movie == movie){
            return;
        }

        current = current->next;
    }

    //yahan adding
    MovieListNode* newNode = new MovieListNode(movie);
    newNode->next = moviesList;
    moviesList = newNode;

}





//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// HashTable
HashTable::HashTable(){

    for (int i = 0; i < TABLE_SIZE; i++){
        table[i] = NULL;
    }

}

HashTable::~HashTable(){

    for (int i = 0; i < TABLE_SIZE; i++){

        while (table[i]){
            ActorNode* temp = table[i];
            table[i] = table[i]->next;
            delete temp;
        }
    }

}

// based on name length actors with same length chained tgt
int HashTable::calc_hash(string name){
    int len;
    int result;
    
    len = name.length();

    if (len < TABLE_SIZE){
        result = len;
    } 
    
    else {
        result = len % TABLE_SIZE;
    }

    return result;

}

// insert_actor - inserts actor movie pair into hash table if actor exists adds movie to their listIf actor doesn't exist creates new ActorNode
 
void HashTable::insert_actor(string actorName, MovieNode* movie){

    int index ;

    index = calc_hash(actorName);
    
    ActorNode* current = table[index];

    while (current){

        if (ignorecase(current->actorName, actorName)){
            current->add_movie(movie); 
            return;
        }

        current = current->next;
   
    }
    
    //found then create a new node
    ActorNode* newActor = new ActorNode(actorName);

    newActor->add_movie(movie);
    newActor->next = table[index];
    table[index] = newActor;

}


int HashTable::count_uniqueactors() {
    int count = 0;
    
    for (int i = 0; i < TABLE_SIZE; i++) {
        ActorNode* current = table[i];
        
        while (current) {
            count++;
            current = current->next;}
    }
    
    return count;

}


//searching for the actor in hash table
ActorNode* HashTable::search_actor(string actorName){

    int index = calc_hash(actorName);
    
    ActorNode* current = table[index];

    while (current){

        if (ignorecase(current->actorName, actorName)){
            return current;
        }

        current = current->next;
    }
    
    return NULL;
}


// show all movies of an actor
void HashTable::display_actormovies(string actorName){

    ActorNode* actor = search_actor(actorName);

    if (actor){
        actor->displayFilmography();
    } 
    
    else {
        cout << "Actor '" << actorName << "' not found" << endl;
    }
}


// display
void HashTable::display_table(){
    int count;

    cout << endl << "=== Hash Table Structure ===" <<endl;

    for (int i = 0; i < TABLE_SIZE; i++){
        if (table[i]){

            cout << "Length " << i << ": ";
            ActorNode* current = table[i];
            count = 0;

            while (current){

                count++;
                current = current->next;
            }

            cout << count << " actors" << endl; }
     }

}




// cleanup movies list
GenreHashNode::~GenreHashNode(){

    while (moviesList){
        MovieListNode* temp = moviesList;
        moviesList = moviesList->next;
        delete temp;}

}

// adds movie to the genre's movie list
void GenreHashNode::add_movie(MovieNode* movie){

    MovieListNode* current = moviesList;

    while (current){
        if (current->movie == movie){
            return;
        }

        current = current->next;
    }
    
    MovieListNode* newNode = new MovieListNode(movie);
    newNode->next = moviesList;
    moviesList = newNode;

}

// shows all movies for this genre

void GenreHashNode::displayMovies(){

    cout << endl <<"--- Movies in genre: " << genreName << " ---" << endl;
    MovieListNode* current = moviesList;

    int count = 1;

    while (current) 
    
    {
        cout << count++ << ". " << current->movie->title<< " (" << current->movie->year << ") - Rating: " << current->movie->rating << endl;
        current = current->next;

    }
}

//initializes the genre table
GenreHashTable::GenreHashTable(){

    for (int i = 0; i < TABLE_SIZE; i++){
        table[i] = NULL;
    }

}



GenreHashTable::~GenreHashTable(){

    for (int i = 0; i < TABLE_SIZE; i++){

        while (table[i]) 
        {
            GenreHashNode* temp = table[i];
            table[i] = table[i]->next;
            delete temp;
        }
    }

}

// based on genre name length 

int GenreHashTable::calc_hash(string name){

    int len ;
    int result;
    len = name.length();
    

    if (len < TABLE_SIZE){
        result = len;
    } 
    
    else {
        result = len % TABLE_SIZE;
    }

    return result;

}

// inserts genre movie pair into hash table
void GenreHashTable::insert_genre(string genreName, MovieNode* movie){
    int index ;
    index = calc_hash(genreName);
    
    GenreHashNode* current = table[index];

    while (current){

        if (ignorecase(current->genreName, genreName)){
            current->add_movie(movie);  
            return;
        }

        current = current->next;
    }
    
    GenreHashNode* newGenre = new GenreHashNode(genreName);//otherwise creates a new genre node
    newGenre->add_movie(movie);
    newGenre->next = table[index];
    table[index] = newGenre;

}
//unique genres in the hash table
int GenreHashTable::count_uniquegenres() {
    int count;
    count = 0;
    
    for (int i = 0; i < TABLE_SIZE; i++) {
        GenreHashNode* current = table[i];
        
        while (current) {
            count++;
            current = current->next;
        }
    }
    
   return count;
}

// searches a genre in genre hash table
GenreHashNode* GenreHashTable::search_genre(string genreName){
    
    int index;
    index = calc_hash(genreName);
    
    GenreHashNode* current = table[index];

    while (current){

        if (ignorecase(current->genreName, genreName)){
            return current;
        }

        current = current->next;
    }

    return NULL;
}

// displays all movies that belong to a given genre

void GenreHashTable::displayGenreMovies(string genreName){

    GenreHashNode* genre = search_genre(genreName);
    
    if (genre){

        genre->displayMovies();
    } 

    else{
        cout << "Genre '" << genreName << "' not found" <<endl;}

}



