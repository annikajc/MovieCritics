#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>

using namespace std;

/* Store Data */
void ReadFromFile(vector<vector<string>> &data);
vector<string> ParseData(string line);
void StoreActorMap(vector<vector<string>> &data, map<string, pair<int, double>> &actorMap);
void StoreMovieMap(vector<vector<string>> &data, map<string, pair<double, vector<string>>> &movieMap);

/* Search Data */
void SearchActor(const string &actorName, map<string, pair<int, double>> &actorMap);
void SearchMovie(const string &movieName, map<string, pair<double, vector<string>>> &movieMap);

/* B+ Tree */
struct Node;
class BPlus;

int main(){
    /* Declare variables here! */
    int menu1 = 0, menu2;
    string inputName; //stores the name of the actor or the movie to search for
    vector<vector<string>> data;
    map<string, pair<int, double>> actorMap;
    map<string, pair<double, vector<string>>> movieMap;

    /* Read data from file */
    ReadFromFile(data);

    /* Displays menu and takes in input from the user */
    cout << "Welcome!\n";

    while(true){
        cout << "\nMenu:\n1. Search for Actor\n2. Search for Movie\nPress any other key to Exit\n";
        cin >> menu1;
        if (menu1 == 1)
            cout << "Which actor would you like to know about?\n";
        else if (menu1 == 2)
            cout << "Which movie would you like to know about?\n";
        else
            break;
        cin.ignore(); // https://www.tutorialspoint.com/what-is-the-use-of-cin-ignore-in-cplusplus
        getline(cin, inputName); // Annika fixed by adding the above line
        cout << "1. Search using Map\n2. Search using B+ Tree\nPress any other key to Exit\n";
        cin >> menu2;

        if (menu1 == 1 && menu2 == 1){
            /* Implement Actor Map */
            StoreActorMap(data, actorMap);
            SearchActor(inputName, actorMap);
        }

        else if (menu1 == 2 && menu2 == 1){
            /* Implement Movie Map */
            StoreMovieMap(data, movieMap);
            SearchMovie(inputName, movieMap);
        }

        else if (menu1 == 1 && menu2 == 2){
            /* Implement Actor B+ Tree */
        }

        else if (menu1 == 2 && menu2 == 2){
            /* Implement Movie B+ Tree */
        }

        else
            break;
    }
}

void ReadFromFile(vector<vector<string>> &data){
    vector<string> lineData; //vector to store all data
    string line; //string to store each line
    //FIXME: NEED RELATIVE PATH NOT ABSOLUTE BUT NOT WORKING OTHERWISE
    ifstream file("/Users/soniacolagiuri/Documents/GitHub/MovieCritics/actorfilms.csv"); //file to read from
    getline (file, line); //first line stores header

    while (getline (file, line)) { //read the file line by line
        lineData.push_back(line); //adds each line to vector of lines
        /* Parse input lines */
        data.push_back(ParseData(line));
    }

    file.close(); //close the file
}

vector<string> ParseData(string line){
    vector<string> parsedLine;
    vector<string> data;

    string word;
    line = line + ','; // add a delimiter to the end
    bool movieNameHasCommas = false;

    for (int i = 0; i < line.size(); i++) { // traverse 'line' from left to right
        if (parsedLine.size() == 2 && line.at(i) == '"' && line.at(i - 1) == ','){ // if next string will be the name of movie, and the next char combo is ',"'
            movieNameHasCommas = true;
            continue;
        }

        if (!movieNameHasCommas){
            if (line.at(i) != ',')
                word += line.at(i);
            else {
                parsedLine.push_back(word);
                word = ""; // reset 'word'
            }
        }

        else{
            if (line.at(i) != '"')
                word += line.at(i);
            else {
                parsedLine.push_back(word);
                word = ""; // reset 'word'
                movieNameHasCommas = false;
                i++;
            }
        }
    }

    //each line contains: 0:Actor,1:ActorID,2:Film,3:Year,4:Votes,5:Rating,6:FilmID
    //delete all data other than actor, film, rating
    data.push_back(parsedLine.at(0));
    data.push_back(parsedLine.at(2));
    data.push_back(parsedLine.at(5));

    return data;
}

void StoreActorMap(vector<vector<string>> &data, map<string, pair<int, double>> &actorMap){
    string currActor = data.at(0).at(0); //first row actor
    int count = 0;
    double totalRating = 0;
    for (auto & i : data) {
        if (i.at(0) == currActor){
            count++;
            totalRating += stod(i.at(2));
        }

        else{
            pair<int, double> currPair({count, totalRating/double(count)});
            actorMap.insert({currActor, currPair}); //add prev data to map

            currActor = i.at(0); // set curr actor to new val
            count = 1; // reset variables
            totalRating = stod(i.at(2));
        }
    }
}

void StoreMovieMap(vector<vector<string>> &data, map<string, pair<double, vector<string>>> &movieMap){
    //iterate through data vector
    //check 2nd index and add that movie to map, store the rating, and store that actor into its vector of strings  - if already in map, DO NOT store rating again

    for (auto & i : data) {
        if (movieMap.find(i.at(1)) == movieMap.end()){ //if not in map yet
            movieMap.insert({i.at(1), {stod(i.at(2)), {i.at(0)}}}); //add it to map
        }

        else{
            pair<double, vector<string>> currPair({movieMap[i.at(1)]});
            vector<string> currVector = currPair.second;
            double rating = stod(i.at(2));
            currVector.push_back(i.at(0));
            pair<double, vector<string>> newPair({rating, currVector});
            movieMap[i.at(1)] = newPair;
        }
    }
}

void SearchActor(const string &actorName, map<string, pair<int, double>> &actorMap){
    bool inDataBase = false;

    string lowerActorName;
    for (char i : actorName) //make input string lowercase
        lowerActorName += tolower(i);

    for (auto & it : actorMap) {
        string lowerName;
        for (char i : it.first) //make current name lowercase
            lowerName += tolower(i);

        if (lowerName == lowerActorName){
            inDataBase = true;
            cout << "Actor " << it.first << " has appeared in " << it.second.first
            << " movies, and has an average rating of " << it.second.second << ".\n";
        }
    }

    if (!inDataBase)
        cout << "Error: This actor does not appear in the database.\n";
}

void SearchMovie(const string &movieName, map<string, pair<double, vector<string>>> &movieMap){
    bool inDataBase = false;

    string lowerMovieName;
    for (char i : movieName) //make input string lowercase
        lowerMovieName += tolower(i);

    for (auto & it : movieMap) {
        string lowerName;
        for (char i : it.first) //make current name lowercase
            lowerName += tolower(i);

        if (lowerName == lowerMovieName){
            inDataBase = true;
            cout << "The movie " << it.first << " has an IMDB rating of " << it.second.first
                 << ", and features the following actors: ";
            for (int i = 0; i < it.second.second.size(); ++i) {
                if (i == it.second.second.size() - 1)
                    cout << "and " << it.second.second.at(i) << ".\n";
                else
                    cout << it.second.second.at(i) << ", ";
            }
        }
    }

    if (!inDataBase)
        cout << "Error: This movie does not appear in the database.\n";
}





/* B+ TREE */

struct Node {
    string name;
    double rating;
    // depending on if it's for actors or movies
    int numMovies;
    vector<string> movieActors;
    Node* left;
    Node* right;

    // Actors
    Node(string name, double rating, int numMovies) : name(name), rating(rating), numMovies(numMovies) {}
    // Movies
    Node(string name, double rating, vector<string>movieActors) : name(name), rating(rating), movieActors() {}
};

class BPlus {
    private:
        int size;

        void destructHelp(Node* node);

    public:
        Node* root;
        BPlus() : root(nullptr), size(0) {} // constructor
        ~BPlus() { destructHelp(root); } // destructor
};

void BPlus::destructHelp(Node* node) { // Annika's destructorHelp function from AVL Project
    destructHelp(node->left); // recursively from the root
    destructHelp(node->right);
    node->left = nullptr; // clears links with children
    node->right = nullptr;
    delete node;
}