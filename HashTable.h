#pragma once
#include "MovieNode.h"

// node in linked list of movies for an actor/genre, so each node points to a movie
 
class MovieListNode {
public:
    MovieNode* movie;
    MovieListNode* next;
    
    MovieListNode(MovieNode* m) : movie(m), next(NULL){}
};

// represents an actor with their filmography. it contains actor name and linked list of their movies 
class ActorNode {
public:

    string actorName;
    MovieListNode* moviesList;  // linked list of movies
    ActorNode* next;           
    
    ActorNode(string name) : actorName(name), moviesList(NULL), next(NULL){}
    ~ActorNode();
    
    void add_movie(MovieNode* movie);
    void displayFilmography();
};


class HashTable {
private:
    static const int TABLE_SIZE = 100;  
    ActorNode* table[TABLE_SIZE];
    
    int calc_hash(string name);
    
public:
    HashTable();
    ~HashTable();
    
    void insert_actor(string actorName, MovieNode* movie);
    ActorNode* search_actor(string actorName);
    void display_table();
    int count_uniqueactors();
    void display_actormovies(string actorName);
    


};

// represents a genre with list of movies

class GenreHashNode {
public:
    string genreName;
    MovieListNode* moviesList; //linked list of movies in that genre
    GenreHashNode* next;
    
    GenreHashNode(string name) : genreName(name), moviesList(NULL), next(NULL){}
    ~GenreHashNode();
    
    void add_movie(MovieNode* movie);
    void displayMovies();
};

// hash table for genres using name length as hash

class GenreHashTable {
private:
    static const int TABLE_SIZE = 100;
    GenreHashNode* table[TABLE_SIZE];
    
    int calc_hash(string name);
    
public:
    GenreHashTable();
    ~GenreHashTable();
    
    void insert_genre(string genreName, MovieNode* movie);
    void displayGenreMovies(string genreName);
    int count_uniquegenres();
    GenreHashNode* search_genre(string genreName);
    
};
