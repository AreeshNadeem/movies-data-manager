#pragma once
#include <iostream>
#include <string>
using namespace std;


class ActorMovieNode;
class GenreNode;
class MovieNode {
public:
    string title;
    string director;
    int height;
    int year;
    double rating;
    int duration;
    ActorMovieNode* actorsList;  
    GenreNode* genresList;
    MovieNode* right;    
    MovieNode* left;//bst pointers 
    //adjacency list for graph
    MovieNode** connections;      
    int connectionCount;
    int connectionCapacity;
    void add_actor(string actorName);
    void add_connections(MovieNode* movie);
    void display_all();
    void add_genre(string genreName);
   
    MovieNode(string t, string dir, int y, double r, int dur);
    ~MovieNode();


};





class ActorMovieNode {
public:
    string actorName;
    ActorMovieNode* next;
    ActorMovieNode(string name) : actorName(name), next(NULL){}



};


class GenreNode {
public:
    GenreNode* next;
    string genreName;  
    GenreNode(string name) : genreName(name), next(NULL){}

};