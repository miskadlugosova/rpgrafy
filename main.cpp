#include <iostream>
#include <fstream>
#include <vector>
#include <set>

using namespace std;

ofstream result;

vector<vector<pair<int, int> > >
        graph;   //graph contains of n rows (each for every vertex), each row contains 3 elements in vector of pairs  - neighbouring vector and the colour of the edge
vector<int> red; // colour == 0
vector<int> blue; //colour == 1
set<vector<int>> sety;
bool existsopt = false;
bool correctfile = true;

int finddif(int i) {   //returns the different coloured edge
    if (graph[i][0].second != graph[i][1].second && graph[i][0].second != graph[i][2].second)
        return 0;
    else if (graph[i][1].second != graph[i][0].second && graph[i][1].second != graph[i][2].second) return 1;
    else return 2;
}

void count(int vertexfrom, int vertex, int colour, int sum) {
    bool exists = false;
    for (int i = 0; i < 3; i++) {
        if (graph[vertex][i].second == colour && graph[vertex][i].first != vertexfrom) {
            count(vertex, graph[vertex][i].first, colour,
                  sum + 1);  //ak mam hranu danej farby a nevedie do vrcholu odkial som prisla
            exists = true;                                              //poznacim si ze som sla dalej
        }
    }
    if (!exists) {
        if (colour == 0) red[sum]++;
        else blue[sum]++;
    }

}

void evaluate() {

    bool correct = true;
    for (int i = 0; i < graph.size(); i++) {
        int dif = finddif(i);
        count(i, graph[i][dif].first, graph[i][dif].second, 1); //pocitaj modre cesty
    }


    for (int i = 0; i < red.size(); i++) {
        if (red[i] != blue[i]) correct = false;
    }

    if (correct) {


        sety.insert(red);

        bool tmp = true;                         //ci kazdy graf ma mnozinu cuest kde najdlhsia je dlzky max 5
         for (int i = 6; i < red.size(); i++) {
             if (red[i] != 0) tmp = false;
         }
         if (tmp) existsopt = true;

    }

    for (int i = 0; i < red.size(); i++) {                   //vynuluje pocty dlzok ciest
        red[i] = 0;
        blue[i] = 0;
    }

}

bool possibleedge(int vertex1, int colour) {
    if ((graph[vertex1][0].second == colour && graph[vertex1][1].second == colour) ||
        (graph[vertex1][0].second == colour && graph[vertex1][2].second == colour) ||
        (graph[vertex1][1].second == colour && graph[vertex1][2].second == colour))
        return false;          //ak existuju (aspon) dve hrany danej farby z vrcholu vertex1 tak by som pridavala tretiu
    else return true;

}


bool existspath(int from, int vertex1, int vertex2, int colour) {

    if (vertex1 == vertex2) return true;    //ak sa mi podarilo prist do vertex2, cesta existovala
    for (int i = 0; i < 3; i++) {
        if (graph[vertex1][i].second == colour && graph[vertex1][i].first != from) {
            return existspath(vertex1, graph[vertex1][i].first, vertex2,
                              colour); //ak z vertex1 vedie hrana danej farby, overim ci existuje cesta z nasledujuceho vrcholu do vertex2
        }
    }
    return false;   //if I connect the vertices with the edge of the colour, I will make a cycle
}

void alg(int vertex, int edge) {


    if (!existsopt) {

        if (graph[vertex][edge].second != -1) {
            if (edge < 2) {
                alg(vertex, edge + 1);
            } else if (vertex < graph.size() - 1) {
                alg(vertex + 1, 0);
            } else if (vertex == graph.size() - 1 && edge == 2) {     //ak sme ofarbili poslendu hranu
                evaluate();
            }
        }    //check if the edge is "blank"
        else {

            if (!existspath(vertex, vertex, graph[vertex][edge].first, 1) &&
                possibleedge(vertex, 1) && possibleedge(graph[vertex][edge].first,
                                                        1)) { //neexistuje cyklus ak tu hranu ofarbim a mozem ofarbit (je to ok aj pre ten druhy vrchol
                graph[vertex][edge].second = 1;

                for (int i = 0; i < 3; i++) {      //ofarbenie aj z druheho vrcholu do vertex
                    if (graph[graph[vertex][edge].first][i].first == vertex)
                        graph[graph[vertex][edge].first][i].second = 1;
                }

                if (edge < 2) {
                    alg(vertex, edge + 1);
                } else if (vertex < graph.size() - 1) {
                    alg(vertex + 1, 0);
                } else if (vertex == graph.size() - 1 && edge == 2) {     //ak sme ofarbili poslendu hranu
                    evaluate();
                }
            }

            if (!existspath(vertex, vertex, graph[vertex][edge].first, 0) &&
                possibleedge(vertex, 0) &&
                possibleedge(graph[vertex][edge].first, 0)) { //neexistuje cyklus ak tu hranu ofarbim a mozem ofarbit
                graph[vertex][edge].second = 0;

                for (int i = 0; i < 3; i++) {
                    if (graph[graph[vertex][edge].first][i].first == vertex)
                        graph[graph[vertex][edge].first][i].second = 0;
                }

                if (edge < 2) alg(vertex, edge + 1);
                else if (vertex < graph.size() - 1) alg(vertex + 1, 0);

                else if (vertex == graph.size() - 1 && edge == 2) {     //ak sme ofarbili poslendu hranu
                    evaluate();
                }
            }
            graph[vertex][edge].second = -1;
            for (int i = 0; i < 3; i++) {      //ofarbenie aj z druheho vrcholu do vertex
                if (graph[graph[vertex][edge].first][i].first == vertex)
                    graph[graph[vertex][edge].first][i].second = -1;
            }
        }
    }

}


int main() {
    int numofver;
    long int l;
    string s, vstup, vystup;
    ifstream myfile;
    cout << "Zadaj cestu k vstupnemu suboru:" << endl;
    cin >> vstup;
    cout << "Zadaj nazov vystupneho suboru" << endl;
    cin >> vystup;
    result.open(vystup);
    myfile.open(vstup);
    myfile >> numofver;
    if (numofver * 3 / 2 % 2 == 1) result << "graf by neobsahoval parny pocet hran" << endl;

    else {
        //int order = 0;
        while (!myfile.eof()) {
            int a;
            //cout << "I am processing graph number " << order << endl;
            //order++;
            existsopt = false;

            myfile >> s >> l >> s;
            //cout << "Graph " << l << endl;
            for (int i = 0; i < numofver; i++) {      // nacita vstup
                red.push_back(0);                    //najdlhsia cesta moze byt dlzky numofver-1
                blue.push_back(0);
                vector<pair<int, int>>
                        tmp;
                for (int j = 0; j < 3; j++) {        // tri susedne vrcholy pre kazdy vektor
                    myfile >> a;
                    pair<int, int> p;
                    p.first = a - 1;                   //v 20-vrcholovych su cislovane od 1 nie od 0
                    p.second = -1;
                    tmp.push_back(p);
                }
                graph.push_back(tmp);
            }

            if (myfile.eof()) continue;             //po precitani posledneho grafu neregistruje eof ale nacitava posledne cislo znovu niekolkokrat
            alg(0, 0);                 //spusti vypocet na danom grafe

            /*result << "Graph " << l << endl;
            for (vector<int> v : sety) {                        //zapisem rozne moznosti ciest do result
                for (int i = 0; i < v.size(); i++) {
                    if (i != 0) result << " " << v[i] / 2;
                    else result << v[i] / 2;
                }
                result << endl;
                result << endl;
            }*/


            if (!existsopt) {            // pre pocet vrcholov 16 ci existuje mnozina ciest kde najdlhsia je max dlzky 5
                correctfile = false;
                result << "I am processing graph number " << l << endl;
                for (vector<int> v : sety) {                        //zapisem rozne moznosti ciest do result
                    for (int i = 0; i < v.size(); i++) {
                        if (i != 0) result << " " << v[i] / 2;
                        else result << v[i] / 2;
                    }
                    result << endl;
                    result << endl;
                }
            }


            for (int i = 0; i < graph.size(); i++) {
                graph[i].clear();
            }
            graph.clear();
            red.clear();
            blue.clear();
            sety.clear();



            /* for (vector<int> v : sety) {                        //zapisem rozne moznosti ciest do result
                 for (int i = 0; i < v.size(); i++) {
                     if (i != 0) result << " " << v[i] / 2;
                     else result << v[i] / 2;
                 }
                 result << endl;
                 result << endl;
             }*/
        }

        if (correctfile) result << "Graphs were correct" << endl;
        else result << "Some of the graphs were incorrect" << endl;
    }

    //cout << "skoncil som" << endl;
    myfile.close();
    result.close();
    return 0;
}