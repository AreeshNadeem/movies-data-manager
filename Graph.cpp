#include "Graph.h"
#include "CSVParser.h"
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#include <iostream>
using namespace std;


// constructor initializes a circular aray
Queue::Queue(int cap) : capacity(cap), size(0), front(0), rear(-1){
    data = new MovieNode*[capacity];

}

// destructor4 frees memory
Queue::~Queue(){
    delete[] data;
}

void Queue::enqueue(MovieNode* movie){

    if (size >= capacity){
        return;  
    }
    
    rear = (rear + 1) % capacity;//wrapping around 
    data[rear] = movie;

    size++;

}


MovieNode* Queue::dequeue(){

    if (isEmpty()){
        return NULL;
    }

    MovieNode* movie = data[front];//returning the first element

    front = (front+1) % capacity;

    size--;
    return movie;

}



bool Queue::isEmpty(){
    return size == 0;

}






//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Graph Implementation

Graph::Graph() : allMovies(NULL), movieCount(0){}


Graph::~Graph(){
    allMovies = NULL;
    movieCount = 0;
}

//checks if two movies have a common actor

bool Graph::shares_actor(MovieNode* m1, MovieNode* m2){

    ActorMovieNode* a1 = m1->actorsList;
    while (a1!=NULL){

        ActorMovieNode* a2 = m2->actorsList;

        while (a2!=NULL){

            if (ignorecase(a1->actorName, a2->actorName)){
                  return true;
            }

            a2 = a2->next;
         }

        a1 = a1->next;
      }


    return false;

}

// checks if two movies have a common genre

bool Graph::shares_genre(MovieNode* m1, MovieNode* m2){

    GenreNode* g1 = m1->genresList;
    while (g1!=NULL){

        GenreNode* g2 = m2->genresList;

        while (g2!=NULL){

            if (ignorecase(g1->genreName, g2->genreName)){
                return true;
            }

            g2 = g2->next;}

        g1 = g1->next;
    }

    return false;

}

//build_graph - Construct graph by creating edges between related movies edge exists if movies share actor OR genre O(n^2) complexity but only run once during initialization
void Graph::build_graph(MovieNode** movies, int count){

    allMovies = movies;
    movieCount = count;
    
    cout << YELLOW  << "Building graph connections......"  << RESET<< endl;

    //checks for adjacent movies
    for (int i = 0; i < count; i++){

        for (int j = i + 1; j < count; j++){

            if (shares_actor(movies[i], movies[j]) || shares_genre(movies[i], movies[j])){

                movies[i]->add_connections(movies[j]);
                movies[j]->add_connections(movies[i]);

            }

        }
        
        // for progress display
        if ((i + 1) % 500 == 0){
            cout << YELLOW << "Processed " << (i + 1) << "/" << count << " movies" << RESET<<  endl;
        }
    }
    
    cout << GREEN << "Graph built successfully!!!" << RESET <<  endl;

}

// finds movie index in the all movies array
int Graph::find_movieindex(MovieNode* movie){

    for (int i = 0; i < movieCount; i++){

        if (allMovies[i] == movie){
            return i;
        }
    }

    return -1;

}

//uses BFS also actor relevance and genre relevance 
void Graph::reccommend_movies(MovieNode* movie, int max_recommendation){
    int actor_count;
    int startingindex;
    double ratingDiff;
    int count;

    if (movie==NULL){
        return;
    }
    
    
    bool* visited = new bool[movieCount];

    for (int i = 0; i < movieCount; i++){
        visited[i] = false;  
    }
    
    startingindex = find_movieindex(movie);  //the movie for which we want recommendation's position in the all movies array

    if (startingindex == -1){  

        delete[] visited;
        return;
    }

    visited[startingindex] = true;
    
    cout << endl; 
    cout << "=== Movies Similar to '" << movie->title << "' ===" << endl;
    cout << "Year: " << movie->year << " | Rating: " << movie->rating << endl;
    
    // Show movies actors and genres for context
    cout << "Actors: ";
    ActorMovieNode* actor = movie->actorsList;
    actor_count = 0;


    while (actor && actor_count < 3){
        cout << actor->actorName;
        if (actor->next && actor_count < 2){
            cout << ", ";
        }
        actor = actor->next;
        actor_count++;
    }

    if (actor_count >= 3 && actor){
        cout << "...";
    }
    cout << endl;
    
    cout << "Genres: ";
    GenreNode* genre = movie->genresList;
    while (genre){
        cout << genre->genreName;
        if (genre->next){
            cout << ", ";
        }

        genre = genre->next;
    }
    cout << endl<< endl;
    


    if (movie->connectionCount == 0){
        cout << "NO similar movies found" << endl;
        delete[] visited;
        return;
    }
    
    //structure to hold movie with its connection strength
    struct ScoredMovie {
        MovieNode* movie;
         bool has_actorconnection;
        bool has_genreConnection;
        int score;
       

    };
    
    //array to score all direct connections
    ScoredMovie* scoredMovies = new ScoredMovie[movie->connectionCount];
    
    //score each connected movie
    for (int i = 0; i < movie->connectionCount; i++){
        MovieNode* connected = movie->connections[i];

        scoredMovies[i].movie = connected;
        scoredMovies[i].score = 0;

        scoredMovies[i].has_actorconnection = false;
        scoredMovies[i].has_genreConnection = false;
        
        //count shared actors (10 points each)
        ActorMovieNode* a1 = movie->actorsList;
        while (a1){
            ActorMovieNode* a2 = connected->actorsList;
            while (a2){
                if (a1->actorName == a2->actorName){
                    scoredMovies[i].score += 10;
                    scoredMovies[i].has_actorconnection = true;
                }
                a2 = a2->next;
            }

            a1 = a1->next;

        }
        

        //count shared genres (2 points each)
        GenreNode* g1 = movie->genresList;
        while (g1){
            GenreNode* g2 = connected->genresList;
            while (g2){
                if (g1->genreName == g2->genreName){
                    scoredMovies[i].score += 2;
                    scoredMovies[i].has_genreConnection = true;
                }
                g2 = g2->next;
            }
            g1 = g1->next;

        }
        
        //bonus for same director (15 points)
        if (!movie->director.empty() && movie->director == connected->director){
            scoredMovies[i].score += 15;
        }
        
        //bonus for similar year (5 points if within 3 years)
        if (movie->year != 0 && connected->year != 0){
            int yearDiff = movie->year - connected->year;

            if (yearDiff < 0){
                yearDiff = -yearDiff;
            }

            if (yearDiff <= 3){
                scoredMovies[i].score += 5;
             }

        }
        
        //bonus for similar rating (3 points if within 1.0)
        
        ratingDiff = movie->rating - connected->rating;
        if (ratingDiff < 0){
            ratingDiff = -ratingDiff;
        }
        if (ratingDiff <= 1.0){
            scoredMovies[i].score += 3;}

    }
    
    int sharedCount;
    int index;
    int genreCount;

    //sort by score (descending) using bubble sort
    for (int i = 0; i < movie->connectionCount - 1; i++){
        for (int j = 0; j < movie->connectionCount - i - 1; j++){

            if (scoredMovies[j].score < scoredMovies[j + 1].score){
                ScoredMovie temp = scoredMovies[j];
                scoredMovies[j] = scoredMovies[j + 1];
                scoredMovies[j + 1] = temp;
            }
         }


    }
    
    //display top recommendations
    cout << "Top Recommendations (sorted by relevance):" << endl << endl;
    count = 0;
    

    for (int i = 0; i < movie->connectionCount && count < max_recommendation; i++){
        MovieNode* connected = scoredMovies[i].movie;
        index = find_movieindex(connected);
        
        if (index != -1 &&!visited[index]){
            visited[index] = true;
            cout << ++count << ". " << connected->title 
                 << " (" << connected->year << ") - Rating: " 
                 << connected->rating << " [Score: " << scoredMovies[i].score << "]" << endl;
            
            // Show connection details
            cout << "   Connection:  ";
            
            if (scoredMovies[i].has_actorconnection){
                cout << "Shared Actors: ";
                ActorMovieNode* a1 = movie->actorsList;
                sharedCount = 0;


                while (a1 && sharedCount < 2){
                    ActorMovieNode* a2 = connected->actorsList;

                    while (a2){
                        if (a1->actorName == a2->actorName){
                            if (sharedCount > 0){
                                cout << ", ";
                            }
                            cout << a1->actorName;
                            sharedCount++;
                            break;
                        }
                        a2 = a2->next;
                    }
                    a1 = a1->next;
                }


                if (sharedCount >= 2 && a1){
                    cout << "...";
                }


            }
            
            if (scoredMovies[i].has_actorconnection && scoredMovies[i].has_genreConnection){
                cout << " | ";
            }
            
            if (scoredMovies[i].has_genreConnection){
                cout << "Shared Genres: ";
                GenreNode* g1 = movie->genresList;
                genreCount = 0;

                while (g1 && genreCount < 2){
                    GenreNode* g2 = connected->genresList;
                    while (g2){
                        if (g1->genreName == g2->genreName){
                            if (genreCount > 0){
                                cout << ", ";}

                            cout << g1->genreName;
                            genreCount++;
                            break;
                        }

                        g2 = g2->next;
                    }
                    g1 = g1->next;

                }



            }
            
            if (!movie->director.empty() && movie->director == connected->director){
                cout << " | Same Director: " << movie->director;
            }
            
            cout << endl << endl;
        }
    }



    
    if (count == 0){
        cout << RED << "No similar movies found" << RESET << endl;
    }
    else{
        cout  << GREEN << "Total recommendations: " << count << RESET <<endl;
    }
    

    delete[] scoredMovies;
    delete[] visited;


}



// debug_connections - Shows connection quality for debugging
 void Graph::debug_connections(MovieNode* movie){
  
    int actorConnections, genreOnlyConnections ,bothConnections;

    actorConnections = 0;
    genreOnlyConnections = 0;
    bothConnections = 0;

    if (!movie){
        return;
    }
    
    cout <<endl << "=== Debug: Connection Analysis for '" << movie->title << "' ===" << endl;
    cout << "Total connections: " << movie->connectionCount << endl<< endl;
    
    bool has_genre;
    bool has_actor;

    for (int i = 0; i < movie->connectionCount; i++){
        MovieNode* connected = movie->connections[i];
        
        has_actor = shares_actor(movie, connected);
        has_genre = shares_genre(movie, connected);
        
        if (has_actor && has_genre){
            bothConnections++;
        }
        
        else if (has_actor){
            actorConnections++;
        }

        else if (has_genre){
            genreOnlyConnections++;
        }


    }
    
    cout << "Connection breakdown:" << endl;
    cout << "  - Both actor AND genre: " << bothConnections << " movies" << endl;
    cout << "  - Actor only: " << actorConnections << " movies " <<endl;
    cout << "  - Genre only: " << genreOnlyConnections << " movies" << endl << endl;
    
    cout << "Top 10 connections:" << endl;
    
    //calculate scores for display
    struct ScoredMovie {
        MovieNode* movie;
        int score;

    };
    
    ScoredMovie* scored = new ScoredMovie[movie->connectionCount];
    
    for (int i = 0; i < movie->connectionCount; i++){
        scored[i].movie = movie->connections[i];
        scored[i].score = 0;
        
        //same scoring logic as reccommend_movies
        ActorMovieNode* a1 = movie->actorsList;

        while (a1){
            ActorMovieNode* a2 = movie->connections[i]->actorsList;
            while (a2){
                if (a1->actorName == a2->actorName){
                    scored[i].score += 10;}
                a2 = a2->next;
            }

            a1 = a1->next;

        }
        
        GenreNode* g1 = movie->genresList;

        while (g1){
            GenreNode* g2 = movie->connections[i]->genresList;

            while (g2){
                if (g1->genreName == g2->genreName){
                    scored[i].score += 2;}
                g2 = g2->next;
            }
            g1 = g1->next;

        }
        
        if (!movie->director.empty() && movie->director == movie->connections[i]->director){
            scored[i].score += 15;
        }

    }
     int displayCount;
    //sort
    for (int i = 0; i < movie->connectionCount - 1; i++){

        for (int j = 0; j < movie->connectionCount - i - 1; j++){
            if (scored[j].score < scored[j + 1].score){
                ScoredMovie temp = scored[j];
                scored[j] = scored[j + 1];
                scored[j + 1] = temp;}
        
            }
     }
    
    //display top 10
    displayCount = (movie->connectionCount < 10) ? movie->connectionCount : 10;
    for (int i = 0; i < displayCount; i++){
        cout << (i + 1) << ". " << scored[i].movie->title << GREEN << " - Score: " << scored[i].score << RESET << endl;
    
    }
    
    delete[] scored;

}


//find_shortestpath - Uses BFS to find shortest path between two movies Tracks parent array to reconstruct path
void Graph::find_shortestpath(MovieNode* start, MovieNode* end){

    int endindex;
    int startingindex;
    bool found;

    if (!start || !end) {
        return;
    }
    
    if (start == end) {
        cout << "Start and end are the same movie!" << endl;
        return;
    }
    
    // Initialize visited and parent arrays
    bool* visited = new bool[movieCount];
    int* parent = new int[movieCount];
    
    for (int i = 0; i < movieCount; i++) {
        visited[i] = false;
        parent[i] = -1;
    }
    
    Queue queue(movieCount);
    startingindex = find_movieindex(start);
    endindex = find_movieindex(end);
    

    bool has_actor;
    bool has_genre;
    bool hasBoth;


    if (startingindex==-1 || endindex==-1) {
        cout << RED << "Movie not found in graph" << RESET << endl;
        delete[] visited;
        delete[] parent;
        return;
    }
    
    queue.enqueue(start);
    visited[startingindex] = true;
    
    found = false;
    
     int path_length ;
     int index;
    //BFS to find path
    while (!queue.isEmpty() && !found) {
        MovieNode* current = queue.dequeue();
        int currentIdx = find_movieindex(current);
        
        //all neighbors
        for (int i = 0; i < current->connectionCount; i++){

            int neighborIdx = find_movieindex(current->connections[i]);
            
            if (neighborIdx != -1 && !visited[neighborIdx]) {
                visited[neighborIdx] = true;
                parent[neighborIdx] = currentIdx;
                queue.enqueue(current->connections[i]);
                
                if (neighborIdx==endindex) {
                    found = true;
                    break;
                }
            }

            }

    }
    
    //reconstruct sara path 
    if (found) {
        path_length = 0;
        index = endindex;

        while (index != -1) {
            path_length++;
            index=parent[index];
        }
        
        //storing the path
        int* path = new int[path_length];
        index = endindex;

        for (int i = path_length - 1; i >= 0; i--) {
            path[i] = index;
            index = parent[index];
        }
        
        //displaying 
        cout << endl;
        cout << "=== Shortest Path ===" << endl;
        cout << "Distance: " << (path_length - 1) << " connections" << endl << endl;

        for (int i = 0; i < path_length; i++) {
            cout << allMovies[path[i]]->title << " (" << allMovies[path[i]]->year << ")";
            
            if (i < path_length-1) {
                cout << endl<< "  |" << endl;
                
                // Show connection details between current and next movie
                MovieNode* current = allMovies[path[i]];
                MovieNode* next = allMovies[path[i+1]];

                cout << "  [Connected via: ";
                has_actor = false;
                has_genre = false;
                hasBoth = false;
                
                //shared
                ActorMovieNode* a1 = current->actorsList;
                while (a1) {
                    ActorMovieNode* a2 = next->actorsList;
                    while (a2) {
                        if (ignorecase(a1->actorName, a2->actorName)) {
                            if (!has_actor) {
                                cout << "Actor: " << a1->actorName;
                                has_actor = true; }
                            break;
                         }
                        a2 = a2->next;
                    }

                    if (has_actor) break;
                    a1 = a1->next;

                }
                
            
                GenreNode* g1 = current->genresList;
                while (g1) {
                    GenreNode* g2 = next->genresList;
                    while (g2) {
                        if (ignorecase(g1->genreName, g2->genreName)) {
                            if (has_actor) {
                                cout << ", Genre: " << g1->genreName;
                            }

                            else {
                                cout << "Genre: " << g1->genreName;
                            }

                            has_genre = true;
                            break;
                        }
                        g2 = g2->next;
                    }

                    if (has_genre) break;
                    g1 = g1->next;


                }
                
                if (!current->director.empty() && current->director == next->director){ //same director
                    if (has_actor || has_genre) {
                        cout << ", Director: " << current->director;
                    }
                    else
                    {
                        cout << "Director: " << current->director;
                    }

                }
                
                cout << "]\n  v" << endl;
            
            }
        }

        cout << endl;
        
        delete[] path;
    }

    else{
        cout << "No path found between these movies." << endl;
    }
    

    
    delete[] visited;
    delete[] parent;


}