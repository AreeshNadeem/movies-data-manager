#pragma once
#include "MovieNode.h"

class Avltree {

private:

    MovieNode* root;
    
    // Helper functions
    int getheight(MovieNode* node);
    int getbalance(MovieNode* node);
    MovieNode* rotate_right(MovieNode* y);
    MovieNode* rotate_left(MovieNode* x);
    MovieNode* insert_helper(MovieNode* node, MovieNode* newMovie);
    MovieNode* search_helper(MovieNode* node, string title, bool findAll, bool& found);
    void inorder_helper(MovieNode* node);
    void destroy_tree(MovieNode* node);
    
    //helpers for the searching
    void searchyear_helper(MovieNode* node, int year);
    void searchratingsrange_helper(MovieNode* node, double minRating, double maxRating);

    //string and movie comparison
    int compare_strings(string a, string b);
    int compare_movies(MovieNode* m1, MovieNode* m2);


    

public:
    Avltree();
    ~Avltree();
    
    void insert(MovieNode* movie);
    MovieNode* search(string title);
    void search_ALL_bytitle(string title);
    void inorder_display();
    MovieNode* getRoot(){ return root; }
    
    //collecting movie nodes
    void collect_allmovies(MovieNode* node, MovieNode** array, int& index);
    int count_noeds(MovieNode* node);

    //searching 
    void search_byyear(int year);
    void search_byratingrange(double minRating, double maxRating);



};