#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <stdlib.h>
#include <map>
#include <set>
#include <climits>
#include <time.h>

using namespace std;

//Compares two cities by distance
bool compare_cities(const string& first, const string& second)
{
    int one = atoi((first.substr(15, first.size())).c_str());
    int two = atoi((second.substr(15, second.size())).c_str());
    return (one < two);
}

int main()
{
    //////////////////////////////////////////////////////////////////////////////////////
    //DATA IMPORT AND PREPERATION
    //////////////////////////////////////////////////////////////////////////////////////
    cout << endl;
    cout << "Starting Step 1" << endl;

    //Import data and make sorted list of edges based on distance
    string input;
    map<string, list<string> > edges;
    ifstream file("map.txt");
    if (file.is_open())
    {
        //Initializing variables
        string prevIn = "ETCW";
        list<string> etcw;
        edges[prevIn] = etcw;
        map<string, list<string> >::iterator pEdge = edges.begin();

        //Inport each line from the map file
        while (getline(file, input))
        {
            //If still getting data for a city, add it to the list.
            if (input.substr(0, 4) == prevIn)
                (pEdge->second).push_back(input);
            //If a new city apears, add a list and start adding to that.
            else
            {
                prevIn = input.substr(0,4);
                list<string> newList;
                edges[prevIn] = newList;
                pEdge = edges.find(prevIn);
                (pEdge->second).push_back(input);
            }
        }
        file.close();
    }

    //Sort each citie's lists by distance
    for (map<string, list<string> >::iterator pEdge = edges.begin(); pEdge != edges.end(); pEdge++)
        (pEdge->second).sort(compare_cities);

    cout << "Step 1 Complete" << endl;
    cout << endl;

    //////////////////////////////////////////////////////////////////////////////////////
    //CREATION OF POSSIBLE SHORTEST PATHS
    //////////////////////////////////////////////////////////////////////////////////////

    cout << "Starting Step 2" << endl;

    /*
    *Only used when randomizing algorithm
    * Amount gives percentage out of 10 that nearest city is chosen as next
    * Loops gives number of times the algorithm is rerun (only effective when amount <10)
    * NOTE: Randomization has shown to produce worse results than nearest city
    */
    srand(time(NULL));
    int randomizeAmount = 10;
    int randomizeLoops = 1;

    //Create place to store current shortest path
    list<string> minPath;
    int minLength = INT_MAX;

    //Outer for loop for multiple tests when randomizing.
    for (int mm = 0; mm < randomizeLoops; mm++)
    {
        //Runs nearest city algorithm using each city as a starting point.
        for (map<string, list<string> >::iterator loop = edges.begin(); loop != edges.end(); loop++)
        {
            //Sets the first city
            list<string> path;
            set<string> used;
            string current = loop->first;
            path.push_back(current);
            used.insert(current);
            int length = 0;

            //Adds cities to the list until there are either no unvisited neighbors
            string nextCity = "";
            while (true)
            {
                //Checks neighbors in increasing distance. If a neighbor hasn't been visited, go to it
                bool added = false;
                for (list<string>::iterator ii = (edges[current]).begin(); ii != (edges[current]).end(); ii++)
                {
                    nextCity = ii->substr(9, 4);
                    if (used.find(nextCity) == used.end() && rand()%10<randomizeAmount)
                    {
                        current = nextCity;
                        path.push_back(current);
                        used.insert(current);
                        length += atoi((ii->substr(15, ii->size())).c_str());
                        added = true;
                        break;
                    }
                }
                if (!added)
                    break;
            }
            
            //If every city has been visited, add the starting city
            bool added = false;
            if (edges.size() == used.size())
            {
                for (list<string>::iterator ii = (edges[current]).begin(); ii != (edges[current]).end(); ii++)
                {
                    nextCity = ii->substr(9, 4);
                    if (nextCity == loop->first)
                    {
                        current = nextCity;
                        path.push_back(current);
                        used.insert(current);
                        length += atoi((ii->substr(15, ii->size())).c_str());
                        added = true;
                        break;
                    }
                }
            }
            //If the starting city was added and a new minimum path was found modify minPath and minLength to reflect it 
            if (added && length < minLength)
            {
                minPath = path;
                minLength = length;
            }
        }
    }
    cout << endl;
    cout << "Step 2 Complete" << endl;
    cout << endl;

    //////////////////////////////////////////////////////////////////////////////////////
    //OUTPUT PATH
    //////////////////////////////////////////////////////////////////////////////////////

    cout << "Starting Step 3" << endl;

    //Write shortest path to a file called result.txt
    ofstream output("result.txt");
    if (output.is_open())
    {
        //Make the starting and ending point in the file ETCW
        list<string>::iterator ii;
        for (ii = minPath.begin(); ii != minPath.end(); ii++)
            if (*ii == "ETCW")
                break;
        list<string>::iterator jj;
        for (jj = ii; jj != minPath.end(); jj++)
        {
            output << *jj << '\n';
        }
        jj = minPath.begin();
        jj++;
        for (; jj != ii; jj++)
        {
            output << *jj << '\n';
        }
        output << "ETCW";
        output.close();
    }
    cout << "Length is: " << minLength << endl;

    cout << "Step 3 Complete" << endl;
    cout << endl;
}