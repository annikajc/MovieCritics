#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

void ReadFromFile(vector<vector<string>> &data);
vector<string> ParseData(string line);

/* B+ Tree */
struct Node;
class BPlus;

int main(){
    /* Declare variables here! */
    int menu1, menu2;
    string inputName; //stores the name of the actor or the movie to search for
    vector<vector<string>> data;

    /* Read data from file */
    ReadFromFile(data); //FIXME: not working

    /* Displays menu and takes in input from the user */
    cout << "Welcome!\n\nMenu:\n1. Search for Actor\n2. Search for Movie\n";
    cin >> menu1;
    if (menu1 == 1)
        cout << "Which actor would you like to know about?\n";
    else if (menu1 == 2)
        cout << "Which movie would you like to know about?\n";
    cin.ignore(); // https://www.tutorialspoint.com/what-is-the-use-of-cin-ignore-in-cplusplus
    getline(cin, inputName); // Annika fixed by adding the above line
    cout << "1. Search using Map\n2. Search using B+ Tree\n";
    cin >> menu2;

    if (menu1 == 1 && menu2 == 1){
        /* Implement Actor Map */
    }

    else if (menu1 == 2 && menu2 == 1){
        /* Implement Movie Map */
    }

    else if (menu1 == 1 && menu2 == 2){
        /* Implement Actor B+ Tree */
    }

    else if (menu1 == 2 && menu2 == 2){
        /* Implement Movie B+ Tree */
    }
}

void ReadFromFile(vector<vector<string>> &data){
    vector<string> lineData; //vector to store all data
    string line; //string to store each line
    ifstream file("actorfilms.csv"); //file to read from
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

    for (char i : line) { // traverse 'line' from left to right
        if (i != ',')
            word += i;
        else {
            parsedLine.push_back(word);
            word = ""; // reset 'word'
        }
    }

    //each line contains: 0:Actor,1:ActorID,2:Film,3:Year,4:Votes,5:Rating,6:FilmID
    //delete all data other than actor, film, rating
    data.push_back(parsedLine.at(0));
    data.push_back(parsedLine.at(2));
    data.push_back(parsedLine.at(5));

    return data;
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