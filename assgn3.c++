#include <iostream>
#include <vector>
#include <string>
#include <queue>
using namespace std;

class edge{
    public:
    int traffic;
    int src;
    int dest;
    int capacity;
    bool blocked;
    int congestion;
    edge(int traffic, int s, int d,int cap,bool b){
        traffic=traffic;
        src=s;
        dest=d;
        capacity=cap;
        blocked=b;
    }
};
class intersection{
    public:
    //this is the counterpart of a node
    vector<edge*> connections;
    edge* greenlight;
    int time;
    intersection(){
        connections.clear();
        greenlight=NULL;
        time=0;
    }
    void addconnection(edge* e){
        connections.push_back(e);
    }

};
class graph{
    public:
    vector<intersection*> intersections;
    graph(){
        intersections.clear();
    }
    void addedge(edge* e){
        intersections[e->src]->addconnection(e);
    }
    void addintersection(intersection* i){
        intersections.push_back(i);
    }
    void printgraph(){
        for(int i=0;i<intersections.size();i++){
            cout<<"Intersection "<<i<<endl;
            for(int j=0;j<intersections[i]->connections.size();j++){
                edge* e=intersections[i]->connections[j];
                cout<<"Edge to "<<e->dest<<" with traffic "<<e->traffic<<" and capacity "<<e->capacity<<endl;
            }
        }
    }
};

class state{
    public:
    graph* g;
    state(graph* gr){
        g=gr;
    }
    void printstate(){
        g->printgraph();
    }
    int calculatecongestion(edge* e){
        int congestion=0;
        for(int i=0;i<g->intersections[e->src]->connections.size();i++){
            edge* temp=g->intersections[e->src]->connections[i];
            if(temp->dest==e->dest){
                congestion=temp->traffic/temp->capacity;
            }
        }
        return congestion;
    }
};


class actions{
    //these are the actions that can be taken to reduce congestion
    // DIVERT_TRAFFIC,
    // CHANGE_SIGNAL,
    // BLOCK_ROAD,
    // DO_NOTHING
    string action;
    actions(string a){
        action=a;
    }
    void executeaction(state* s){
        if(action=="DIVERT_TRAFFIC"){
            
        }
        else if(action=="CHANGE_SIGNAL"){
            //code to change signal
        }
        else if(action=="BLOCK_ROAD"){
            //code to block road
        }
        else if(action=="DO_NOTHING"){
            //code to do nothing
        }
    }
    
};
class planner{
    double maxCongestion=0.8; // defining a threshold for congestion
    int tickrate=20; // defining a tick rate for the simulation, defines no. of vehicles passing through the route per unit time
};

