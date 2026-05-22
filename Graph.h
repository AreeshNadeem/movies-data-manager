#pragma once
#include "MovieNode.h"
//#include "CSVParser.h"

// for BFS Traversal
class Queue {
private:
    MovieNode** data;
    int front;
    int  rear;
    int  size;
    int capacity;
    
public:
    Queue(int cap);
    ~Queue();
    void enqueue(MovieNode* movie);
    bool isEmpty();
    MovieNode* dequeue();
};

// represents movie relationship graph each movie is a vertex edges represent shared actors/genres as BFS for shortest path finding and recommendations

class Graph {
private:
    MovieNode** allMovies;  
    int movieCount;

   
    bool shares_genre(MovieNode* m1, MovieNode* m2);
    bool shares_actor(MovieNode* m1, MovieNode* m2);
    
    int find_movieindex(MovieNode* movie);
    
public:
    Graph();
    ~Graph();
    
    void build_graph(MovieNode** movies, int count);
    void reccommend_movies(MovieNode* movie, int max_recommendation);
    void find_shortestpath(MovieNode* start, MovieNode* end);
    void debug_connections(MovieNode* movie); 
};