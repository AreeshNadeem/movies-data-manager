#include "MovieNode.h"




// sets up empty linked lists for actorsgenres and graph connections
MovieNode::MovieNode(string t, string dir, int y, double r, int dur): title(t), director(dir), year(y), rating(r), duration(dur),actorsList(NULL), genresList(NULL),  left(NULL), right(NULL), height(1), connections(NULL), connectionCount(0), connectionCapacity(10){

    connections = new MovieNode*[connectionCapacity];

}


MovieNode::~MovieNode(){
    while (actorsList){

        ActorMovieNode* temp = actorsList;
        actorsList = actorsList->next;
        delete temp;

     }
    
    while (genresList){
        GenreNode* temp = genresList;
        genresList = genresList->next;
        delete temp;

   }
    

    if (connections){
        delete[] connections;}


}



//add_actor - adds an actor to this movie's actor listcreates new actormovieNode and prepends to linked list
void MovieNode::add_actor(string actorName){
    ActorMovieNode* newNode = new ActorMovieNode(actorName);
    newNode->next = actorsList;

    actorsList = newNode;

}


/* add_genre -adds a genre to this movie's genre list new GenreNode banata and prepends to linked list*/

void MovieNode::add_genre(string genreName){
    GenreNode* newNode = new GenreNode(genreName);
    newNode->next = genresList;

    genresList = newNode;

}

//add_connectionss - adds edge to another movie in graph expand kartay hoaye dynamic array agar needed
void MovieNode::add_connections(MovieNode* movie){
    for (int i = 0; i < connectionCount; i++){//already?

        if (connections[i] == movie){
            return;}

    }
    

    if (connectionCount >= connectionCapacity){//expanding array agar zarorat
        connectionCapacity *= 2;

        MovieNode** newConnections = new MovieNode*[connectionCapacity];

        for (int i = 0; i < connectionCount; i++){

            newConnections[i] = connections[i];
         }


        delete[] connections;
        connections = newConnections;

    }
    
    connections[connectionCount++] = movie;


}



void MovieNode::display_all(){
    cout << "SHOWING ALL:" <<endl;
    cout << endl <<"------------------------------------------" << endl;
    cout <<  "Title: " << title << endl;
    
    //no year it has 0 in data set so just show n/a instead of year when that data being showed 
    cout << "Year: ";
    if (year == 0){
        cout <<"N/A" << endl;
     }
    
    else {
        cout << year << endl;
     
     }
    
    cout << "Rating: " << rating << endl;
    cout << "Duration: " << duration << " min" << endl;

    cout << "Director: " << director << endl;
    
    cout << "Actors: ";

    ActorMovieNode* actor = actorsList;
    
    while (actor){
        cout << actor->actorName;
        if (actor->next){
            cout << ", ";
         }

        actor = actor->next;
    
    }
    cout << endl;
    
    cout << "Genres: ";
    GenreNode* genre = genresList;

    while (genre){
        cout << genre->genreName;
        if (genre->next){
            cout << ", ";}

        genre = genre->next;

    }


    cout << endl;

    cout << "----------------------------------------" << endl;



}
