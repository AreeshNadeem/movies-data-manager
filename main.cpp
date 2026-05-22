#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
//#include <ctime>
#include "HashTable.h"
#include "Graph.h"
#include "MovieNode.h"
#include "AVLTree.h"
#include "CSVParser.h"


#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

using namespace std;

// opens file, read it creates an avl tree and hash tables for actors and genres
int loadMoviesFromCSV(const string& filename, Avltree& tree, HashTable& actorTable, GenreHashTable& genreTable){
   
    ifstream file(filename);

    if (!file.is_open()){
       cout << RED << "ERROR: Could not open file " << filename << RESET << endl;

        return 0;

    }
    
    CSVParser parser;
    string line;
    int movieCount = 0;
    int skippedCount = 0;
    string* genres;
    int genreCount;
    int year;
    double rating;
    int duration;
    int pos;
    int fieldCount;

    getline(file, line);
    cout << YELLOW << "!! Loading movies from CSV..." << RESET << endl;

    
    while (getline(file, line)){

        if (line.empty()){
            continue;
        }
        
        pos = 0;
        
        //read fields from the CSV line into an array
        string fields[30];  // Enough space for all expected columns
        fieldCount = 0;
        
        while (pos < (int)line.length() && fieldCount < 30){
            fields[fieldCount++] = parser.parse_field(line, pos);
        }
        
        // konsi positions haim
        //1 : director_name
        //3 : duration
        //9 : genres
        //10 : actor_1_name
        //6 : actor_2_name
        //14 : actor_3_name
        //11 : movie_title
        //23 : title_year
        //25 : imdb_score
        string title = "";
        string director = "";
        string yearStr = "";
        string ratingStr = "";
        string durationStr = "";
        string actorsStr = "";
        string genresStr = "";
        
        if (fieldCount > 11 && !fields[11].empty()){
            title = fields[11];
         }
        
         if (title.empty()){
            cout<< MAGENTA << "SKIPPED ROW " << (movieCount + skippedCount + 1) << ":Empty title after parsing " << RESET <<  endl;
            cout << "Raw line: " << line.substr(0, 100) << "..." << endl;
            skippedCount++;
            continue;
         }
         

        if (fieldCount > 1 && !fields[1].empty()){
            director = fields[1];
        }
        if (fieldCount > 23 && !fields[23].empty()){
            yearStr = fields[23];
         }
        if (fieldCount > 25 && !fields[25].empty()){
            ratingStr = fields[25];
        }
        if (fieldCount > 3 && !fields[3].empty()){
            durationStr = fields[3];}


        if (fieldCount > 9 && !fields[9].empty()){
            genresStr = fields[9];
       }
        
        if (fieldCount > 10 && !fields[10].empty()){//actors build
            actorsStr = fields[10];
        
        }
        
        if (fieldCount > 6 && !fields[6].empty()){
            if (!actorsStr.empty()){
                actorsStr += "|";
         }
            
            actorsStr += fields[6];
        
        }
        if (fieldCount > 14 && !fields[14].empty()){
            if (!actorsStr.empty()){
                actorsStr += "|";}
            actorsStr += fields[14];
        }
        //conv to strings 
        rating = 0.0;
        duration = 0;
        year = 0;
        
        if (!yearStr.empty()){
            stringstream ss(yearStr);
            ss >> year;
         }
        
        if (!ratingStr.empty()){
            stringstream ss(ratingStr);
            ss >> rating;
        }
        

        if (!durationStr.empty()){
            stringstream ss(durationStr);
            ss >> duration;
         }
        //new movie node from parsed data
        MovieNode* movie = new MovieNode(title, director, year, rating, duration);
        
        

        if (!actorsStr.empty()){//split the actors string and dd actors to the movie update the actor hash table
            string* actors;
            int actorCount;
            parser.split_bycomma(actorsStr, '|', actors, actorCount);
            
            for (int i = 0; i < actorCount; i++){
                if (!actors[i].empty()){
                    movie->add_actor(actors[i]);
                    actorTable.insert_actor(actors[i], movie);
              }
            }

            delete[] actors;



        }
        
        
        if (!genresStr.empty()){
            //split the actors string and dd actors to the movie update the actor hash table
            parser.split_bycomma(genresStr, '|', genres, genreCount);
            
            for (int i = 0; i < genreCount; i++){
                if (!genres[i].empty()){
                    movie->add_genre(genres[i]);
                    genreTable.insert_genre(genres[i], movie);
                 }

             }

            delete[] genres;


        }
        
        //store the movie in the AVL tree if a movie with the same title and year already exists 
        tree.insert(movie);
        movieCount++;
        if (movieCount % 500 == 0){
            cout << YELLOW << "Loaded " << movieCount << " movies..." << RESET << endl;
 }



    }
    


    file.close();
    cout << GREEN << "✔ Successfully loaded " << movieCount << " movies!" << RESET << endl;

    cout << "Skipped " << skippedCount << " rows with empty titles" << endl;


    return movieCount;


}



// =================================================
void displayMenu(){
    cout << "\n\033[36m=====================================\033[0m\n";
    cout << "\033[33m           MovieDB Engine\033[0m\n";
    cout << "\033[36m=====================================\033[0m\n";

    cout << "\033[32m1.\033[0m  Search movie by title\n";
    cout << "\033[32m2.\033[0m  Search movies by actor\n";
    cout << "\033[32m3.\033[0m  Search movies by genre\n";
    cout << "\033[32m4.\033[0m  Search movies by year\n";
    cout << "\033[32m5.\033[0m  Search movies by rating range\n";
    cout << "\033[32m6.\033[0m  Get movie recommendations\n";
    cout << "\033[32m7.\033[0m  Find shortest path between movies\n";
    cout << "\033[35m9.\033[0m  Debug movie connections\n";
    cout << "\033[31m8.\033[0m  Exit\n";

    cout << "\033[36m=====================================\033[0m\n";
    cout << endl;
    cout << "\033[1mEnter choice:\033[0m ";
}





int main(){
    Avltree movieTree;
    Graph movieGraph;
    GenreHashTable genreHashTable;
    HashTable actorHashTable;
    string title2;string title1;
    double minR, maxR;
    string title;
    string actorName;
    string genreName;
    int year, num,choice;
    bool running = true;
    int loadedRows , uniqueMovies , index;
    

    cout << endl;
    cout << "Enter CSV filename or just press Enter for 'movie_metadata.csv': ";
    string filename;
    getline(cin, filename);

    if (filename.empty()){
        filename = "movie_metadata.csv";}



  //  clock_t startLoad = clock();
    
    loadedRows = loadMoviesFromCSV(filename, movieTree, actorHashTable, genreHashTable);

    //clock_t endLoad = clock();
    //double loadTime = double(endLoad - startLoad) / CLOCKS_PER_SEC;
    
    if (loadedRows == 0){
        cout << "No movies loaded. Exiting." << endl;
        return 1;

    }

   // cout << GREEN << "CSV Loading time: " << loadTime << " seconds" << RESET << endl;

    int  uniqueActors; 
    int uniqueGenres;

    //count how many unique movies stored in the avl
    uniqueMovies = movieTree.count_noeds(movieTree.getRoot());
    cout << MAGENTA << "Unique movies stored in AVL: " << uniqueMovies << RESET << endl;

    uniqueActors = actorHashTable.count_uniqueactors();
    uniqueGenres = genreHashTable.count_uniquegenres();
    cout << MAGENTA << "Unique actors: "<< uniqueActors << RESET << endl;
    cout << MAGENTA << "Unique genres: " << uniqueGenres << RESET << endl;

    cout <<MAGENTA << "Total rows processed: " << loadedRows << RESET << endl;;
    cout << MAGENTA << "Difference (duplicates): " << (loadedRows - uniqueMovies) << RESET << endl;
    
    index = 0;
    MovieNode** allMovies = new MovieNode*[uniqueMovies]; //copy all movie pointers from the avl into an array graph isko use karaye ga to build connections.
    movieTree.collect_allmovies(movieTree.getRoot(), allMovies, index);
    movieGraph.build_graph(allMovies, uniqueMovies);    //build the graph jab using the list of all movies
    


    //menu 
       
    while (running){
        displayMenu();
        cin >> choice;
        cin.ignore();
        

        switch (choice){

            case 1: {
                cout <<endl <<  "Enter movie title: ";
                getline(cin, title);
                movieTree.search_ALL_bytitle(title);
                /*
                clock_t startSearch = clock();
                movieTree.search_ALL_bytitle(title);
                clock_t endSearch = clock();
                
                double searchTime = double(endSearch - startSearch) / CLOCKS_PER_SEC;
                cout << "\nSearch time: " << searchTime << " seconds" << endl;
                */
                break;
                

            }
            
            case 2: {
                cout << endl;
                cout << "Enter actor name: ";
                getline(cin, actorName);
                actorHashTable.display_actormovies(actorName);

                /*
                 clock_t startSearch = clock();
                actorHashTable.displayActorMovies(actorName);
                clock_t endSearch = clock();

                double searchTime = double(endSearch - startSearch) / CLOCKS_PER_SEC;
                cout << "\nSearch time: " << searchTime << " seconds" << endl;
                */
                break;


            }
            
            case 3: {
                cout << endl << "Enter genre name: ";
                getline(cin, genreName);  
                genreHashTable.displayGenreMovies(genreName);
                /*
                clock_t startSearch = clock();
                genreHashTable.displayGenreMovies(genreName);
                clock_t endSearch = clock();
      
                double searchTime = double(endSearch - startSearch) / CLOCKS_PER_SEC;
                cout << "\nSearch time: " << searchTime << " seconds" << endl;
                */
                break;
            }

            case 4: {
                cout << endl;
                cout << "Enter year or enter '0' for movies without date given: ";
                cin >> year;
                cin.ignore();
                
                movieTree.search_byyear(year);
                /* 
                clock_t startSearch = clock();
                movieTree.search_byyear(year);
                clock_t endSearch = clock();

                double searchTime = double(endSearch - startSearch) / CLOCKS_PER_SEC;
                cout << "\nSearch time: " << searchTime << " seconds" << endl;*/
                break;
            }
            
            case 5: {
                cout << endl;
                cout << "Enter minimum rating: ";
                cin >> minR;
                cout << "Enter maximum rating: ";
                cin >> maxR;
                cin.ignore();
                movieTree.search_byratingrange(minR, maxR);

                /*clock_t startSearch = clock();
                movieTree.search_byratingrange(minR, maxR);
                clock_t endSearch = clock();

                double searchTime = double(endSearch - startSearch) / CLOCKS_PER_SEC;
                cout << "\nSearch time: " << searchTime << " seconds" << endl;*/
                            

                break;

            }


            case 6:
             {
                cout << endl;
                cout << "Enter movie title for recommendations:  ";
                getline(cin, title);
                
                MovieNode* movie = movieTree.search(title);
                if (movie){
                    cout << endl;
                    cout << "How many recommendations? :";
                    cin >> num;
                    cin.ignore();
                    movieGraph.reccommend_movies(movie, num);


                   /* clock_t startRec = clock();
                    movieGraph.recommendMovies(movie, num);
                    clock_t endRec = clock();

                    double recTime = double(endRec - startRec) / CLOCKS_PER_SEC;
                    cout << "\nRecommendation generation time: " << recTime << " seconds" << endl;
                */
                
                    } 
                
                else {
                    cout <<RED << "----Movie not found----" << RESET<< endl;
                }


                break;
            }

            case 7: {
                cout << endl;
                cout << "Enter first movie title: ";
                getline(cin, title1);
                cout << "Enter second movie title: ";
                getline(cin, title2);
                
                MovieNode* movie1 = movieTree.search(title1);
                MovieNode* movie2 = movieTree.search(title2);
                
                if (movie1 && movie2){
                    movieGraph.find_shortestpath(movie1, movie2);
                    /*clock_t endPath = clock();

                    double pathTime = double(endPath - startPath) / CLOCKS_PER_SEC;
                    cout << "\nPath finding time: " << pathTime << " seconds" << endl;*/
                } 
                
                else
             {
                    cout <<RED << "One or maybe both movies not found." << RESET << endl;
                }
                
                break;

            }
            
            case 9: {
                cout << endl;
                cout << "Enter movie title to debug: ";
                getline(cin, title);
                
                MovieNode* movie = movieTree.search(title);
                if (movie){
                    movieGraph.debug_connections(movie);
                }
                 
                
                else {
                    cout << RED<< "---Movie not found=--" << RESET <<endl;
                }
                break;
            }

            case 8: 
            {
                cout << RED << "Exiting!!! byee  Thank you!" << RESET << endl;
                running = false;
                break;

            }
            


            default:
                cout << "Invalid choice please try again." << endl;}



    }
    




    //free the array that holds pointers to movies (movies themselves are managed elsewhere)
    delete[] allMovies;
    




    return 0;


}
