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
void StoreActorVec(vector<vector<string>> &data, vector<tuple<string, double, int>> &actorVec);
void StoreMovieVec(vector<vector<string>> &data, vector<pair<string, double>> &movieVec);

/* Search Data */
void SearchActor(const string &actorName, map<string, pair<int, double>> &actorMap);
void SearchMovie(const string &movieName, map<string, pair<double, vector<string>>> &movieMap);

/* Sort Data */
void TopActorsPrint(vector<tuple<string, double, int>> &vec);
void TopMoviesPrint(vector<pair<string, double>> &vec);
// QuickSort - https://www.geeksforgeeks.org/cpp-program-for-quicksort/#
int ActorPartition(vector<tuple<string, double, int>> &vec, int low, int high);
void ActorQuickSort(vector<tuple<string, double, int>> &actorQS, int low, int high);
int MoviePartition(vector<pair<string, double>> &vec, int low, int high);
void MovieQuickSort(vector<pair<string, double>> &movieQS, int low, int high);


int main(){
    /* Declare variables here! */
    int menu1 = 0, menu2 = 0;
    string inputName; //stores the name of the actor or the movie to search for
    vector<vector<string>> data;
    map<string, pair<int, double>> actorMap;
    map<string, pair<double, vector<string>>> movieMap;
    vector<tuple<string, double, int>> actorMS;
    vector<pair<string, double>> movieMS;
    vector<tuple<string, double, int>> actorQS;
    vector<pair<string, double>> movieQS;
    bool isActorMS, isMovieMS, isActorQS, isMovieQS = false; // keep track if vector has already been sorted
    
    /* Read data from file */
    ReadFromFile(data);

    /* Displays menu and takes in input from the user */
    cout << "Welcome!\n";

    while(true){
        cout << "\nMenu:\n1. Search for an Actor\n2. Search for a Movie\n3. Print Top 100 Actors\n4. Print Top 100 Movies\nPress any other key to Exit\n";
        cin >> menu1;
        if (menu1 == 1 || menu1 == 2) {
            if (menu1 == 1)
                cout << "Which actor would you like to know about?\n";
            else if (menu1 == 2) {
                cout << "Which movie would you like to know about?\n";
            }
            cin.ignore();
            getline(cin, inputName);
        }

        else if (menu1 == 3 || menu1 == 4){
            cout << "Which sorting algorithm would you like to use?\n1. Merge Sort\n2. Quick Sort\n";
            cin >> menu2;
        }

        else
            break;

        if (menu1 == 1){
            /* Implement Actor Map */
            StoreActorMap(data, actorMap);
            SearchActor(inputName, actorMap);
            //implement printing individual rank
        }

        else if (menu1 == 2){
            /* Implement Movie Map */
            StoreMovieMap(data, movieMap);
            SearchMovie(inputName, movieMap);
            //implement printing individual rank
        }

        else if (menu1 == 3 && menu2 == 1){
            /* Implement Merge Sort Actor Ranking */

            //write a function that stores actors in order of ranking using merge sort
            if(!isActorMS) { // prevents from sorting an already sorted vector
                StoreActorVec(data, actorMS);
                // merge sort
                isActorMS = true;
            }
            //break ties with actor that appears in more movies!
            //write a function that prints out a ranked list of the top 100 actors
            TopActorsPrint(actorMS);
        }

        else if (menu1 == 3 && menu2 == 2){
            /* Implement Quick Sort Actor Ranking */
            if (!isActorQS) {
                StoreActorVec(data, actorQS);
                ActorQuickSort(actorQS, 0, actorQS.size() - 1);
                isActorQS = true;
            }
            TopActorsPrint(actorQS);
        }

        else if (menu1 == 4 && menu2 == 1){
            /* Implement Merge Sort Movie Ranking */

            //write a function that stores movies in order of ranking using merge sort
            if (!isMovieMS) {
                StoreMovieVec(data, movieMS);
                mergeSortMovies(movieMS, 0, movieMS.size() - 1);
                // merge sort
                isMovieMS = true;
            }
            //write a function that prints out a ranked list of the top 100 movies
            TopMoviesPrint(movieMS);
        }

        else if (menu1 == 4 && menu2 == 2){
            /* Implement Quick Sort Movie Ranking */

            //write a function that stores movies in order of ranking using quick sort
            if (!isMovieQS) {
                StoreMovieVec(data, movieQS);
                MovieQuickSort(movieQS, 0, movieQS.size() - 1);
                isMovieQS = true;
            }
            TopMoviesPrint(movieQS);
        }

        else
            break;
    }
}

void ReadFromFile(vector<vector<string>> &data){
    vector<string> lineData; //vector to store all data
    string line; //string to store each line
    // Below should work now
    ifstream file("cmake-build-debug/actorfilms.csv"); //file to read from
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
    data.push_back(parsedLine.at(0)); //actor
    data.push_back(parsedLine.at(2)); //film
    data.push_back(parsedLine.at(5)); //rating

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
            vector<string> currVector = currPair.second; //get actor vector
            double rating = stod(i.at(2));
            currVector.push_back(i.at(0)); //add to actor vector
            pair<double, vector<string>> newPair({rating, currVector});
            movieMap[i.at(1)] = newPair;
        }
    }
}

void StoreActorVec(vector<vector<string>> &data, vector<tuple<string, double, int>> &actorVec) { // store for sorting
    string currActor = data.at(0).at(0); //first row actor
    int count = 0;
    double totalRating = 0;
    for (auto & i : data) {
        if (i.at(0) == currActor){
            count++;
            totalRating += stod(i.at(2));
        }

        else{
            actorVec.push_back(make_tuple(currActor, totalRating/double(count), count)); // add to vector

            currActor = i.at(0); // set curr actor to new val
            count = 1; // reset variables
            totalRating = stod(i.at(2));
        }
    }
}

void StoreMovieVec(vector<vector<string>> &data, vector<pair<string, double>> &movieVec) {
    for (auto & i : data) {
        bool movieFound = false;
        for (int j = 0; j < movieVec.size(); j++) {
            if (i.at(1) == movieVec.at(j).first) {
                movieFound = true;
                break;
            }
        }
        if (!movieFound) {
            movieVec.push_back(make_pair(i.at(1), stod(i.at(2))));
        }
    }
    cout << "StoreMovieVec Successful" << endl;
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
            if (it.second.first == 1)
                cout << "Actor " << it.first << " has appeared in " << it.second.first
                << " movie, and has an average rating of " << it.second.second << ".\n";
            else
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

void TopActorsPrint(vector<tuple<string, double, int>> &vec) { // prints top 100 actors
    cout << "Top 100 Actors:\n";
    int num = 1;
    for (int i = 0; i < 100; i++) {
        cout << num << ". " << get<0>(vec[i]) << " - " << get<1>(vec[i]) << endl; // #. Actor, Rating
        num++;
    }
}

void TopMoviesPrint(vector<pair<string, double>> &vec) {
    cout << "Top 100 Movies:\n";
    int num = 1;
    for (int i = 0; i < 100; i++) {
        cout << num << ". " << vec[i].first << " - " << vec[i].second << endl; // #. Movie, Rating
        num++;
    }
}

int ActorPartition(vector<tuple<string, double, int>> &vec, int low, int high) { // swaps based on where pivot is
    string pivotName = get<0>(vec[low]);
    double pivotRating = get<1>(vec[low]);
    int pivotMovies = get<2>(vec[low]);
    int count = 0;

    for (int i = low + 1; i <= high; i++) { // find where pivot lies
        if (get<1>(vec[i]) >= pivotRating)
            count++;
    }

    int pivotIndex = low + count;
    swap(vec[pivotIndex], vec[low]); // put pivot in its index
    int i = low, j = high;

    while (i < pivotIndex && j > pivotIndex) {
        while (get<1>(vec[i]) >= pivotRating) // find 1st rating on the left less than pivot
            i++;
        while (get<1>(vec[j]) < pivotRating) // find 1st rating on the right greater than pivot
            j--;
        if (i < pivotIndex && j > pivotIndex) // swap if they're still on their respective sides
            swap(vec[i++], vec[j--]);
    }

    for (int i = pivotIndex - 1; i >= low; i--) {
        if (get<1>(vec[i]) == pivotRating) { // same rating, different number of movies
            if (get<2>(vec[i]) > pivotMovies) { // more movies = higher rating
                swap(vec[i], vec[pivotIndex]);
                pivotIndex = i;
            }
        }
        else
            break;
    }

    return pivotIndex;
}

void ActorQuickSort(vector<tuple<string, double, int>> &actorQS, int low, int high) { // quick sort for actor vector
    cout << "Sorting...\n";
    if (low < high) { // prevents from infinite recursion
        int pivotIndex = ActorPartition(actorQS, low, high);
        ActorQuickSort(actorQS, low, pivotIndex - 1); // bottom half
        ActorQuickSort(actorQS, pivotIndex + 1, high); // top half
    }
    cout << "Sort successful\n";
}

int MoviePartition(vector<pair<string, double>> &vec, int low, int high) { // swaps based on where pivot is
    string pivotName = vec[low].first;
    double pivotRating = vec[low].second;
    int count = 0;

    for (int i = low + 1; i <= high; i++) { // find where pivot lies
        if (vec[i].second >= pivotRating)
            count++;
    }

    int pivotIndex = low + count;
    swap(vec[pivotIndex], vec[low]); // put pivot in its index
    int i = low, j = high;

    while (i < pivotIndex && j > pivotIndex) {
        while (vec[i].second >= pivotRating) // find 1st rating on the left less than pivot
            i++;
        while (vec[j].second < pivotRating) // find 1st rating on the right greater than pivot
            j--;
        if (i < pivotIndex && j > pivotIndex) // swap if they're still on their respective sides
            swap(vec[i++], vec[j--]);
    }

    for (int i = pivotIndex - 1; i >= low; i--) {
        if (vec[i].second == pivotRating) { // same rating, different name
            if (vec[i].first > pivotName) { // swap if alphabetical order is not maintained
                swap(vec[i], vec[pivotIndex]);
                pivotIndex = i;
            }
        }
        else {
            break;
        }
    }

    return pivotIndex;
}

// cannot be replaced w/ template class bc of the partition method called
void MovieQuickSort(vector<pair<string, double>> &movieQS, int low, int high) { // quick sort for movie vector
    if (low < high) { // prevents from infinite recursion
        int pivotIndex = MoviePartition(movieQS, low, high);
        MovieQuickSort(movieQS, low, pivotIndex - 1); // bottom half
        MovieQuickSort(movieQS, pivotIndex + 1, high); // top half
    }
}


void mergeMovies(vector<pair<string, double>> &movieMS, int left, int mid, int right) {

        int n1 = mid - left + 1;
        int n2 = right - mid;
        int X[n1], Y[n2];

        for(int i = 0; i < n1; i++) {
            X[i] = movieMS[left+1].second;
        }
        for(int j = 0; j < n2; j++) {
            Y[j] = movieMS[mid + 1 + j].second;
        }
        int i, j, k;
        i = 0;
        j = 0;
        k = left;

        while(i < n1 && j < n2) {
            if(X[i] <= Y[j]) {
                movieMS[k].second = X[i];
                i++;
            } else {
                movieMS[k].second = Y[j];
                j++;
            }
            k++;
        }
        while(i < n1) {
            movieMS[k].second = X[i];
            i++;
            k++;
        }
        while(j < n2) {
            movieMS[k].second = Y[j];
            j++;
            k++;
        }
    }

void mergeSortMovies(vector<pair<string, double>> &movieMS, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortMovies(movieMS, left, mid);
        mergeSortMovies(movieMS, mid+1, right);

        mergeMovies(movieMS, left, mid, right);

    }
}
