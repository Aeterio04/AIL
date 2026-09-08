#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <cfloat>
using namespace std;

class edge{
    public:
    int traffic;
    int src;
    int dest;
    int capacity;
    bool blocked;
    int congestion;
    edge(int traf, int s, int d,int cap,bool b){
        traffic=traf;
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
    float calculatecongestion(edge* e){
        float congestion=0;
        for(int i=0;i<g->intersections[e->src]->connections.size();i++){
            edge* temp=g->intersections[e->src]->connections[i];
            if(temp->dest==e->dest){
                congestion=(float)temp->traffic/(float)temp->capacity;
            }
        }
        return congestion;
    }
    void displayState(){

    cout << "\n========================================\n";
    cout << "              CURRENT STATE             \n";
    cout << "========================================\n";

    for(int i = 0; i < g->intersections.size(); i++){

        intersection* inter = g->intersections[i];

        cout << "\nIntersection " << i << "\n";
        cout << "----------------------------------------\n";

        // Show which road currently has green light
        if(inter->greenlight != NULL){
            cout << "Green Light : "
                 << inter->greenlight->src
                 << " -> "
                 << inter->greenlight->dest
                 << "\n";

            cout << "Signal Time : "
                 << inter->time
                 << "\n";
        }
        else{
            cout << "Green Light : NONE\n";
            cout << "Signal Time : 0\n";
        }

        cout << "\nOutgoing Roads:\n";

        for(int j = 0; j < inter->connections.size(); j++){

            edge* e = inter->connections[j];

            float congestion = calculatecongestion(e);

            cout << "  "
                 << e->src << " -> " << e->dest
                 << " | Traffic: " << e->traffic
                 << "/" << e->capacity
                 << " | Congestion: " << congestion * 100 << "%"
                 << " | ";

            if(e->blocked){
                cout << "BLOCKED";
            }
            else{
                cout << "OPEN";
            }

            if(inter->greenlight == e){
                cout << " | GREEN";
            }

            cout << "\n";
        }
    }

    cout << "\n========================================\n";
}

};

enum ActionType {
    DIVERT_TRAFFIC,
    CHANGE_SIGNAL,
    BLOCK_ROAD,
    DO_NOTHING
};

class actions{
    //these are the actions that can be taken to reduce congestion
    //HTN
    //Reduce congestion actions:
    // DIVERT_TRAFFIC,
    //-find congested road
    //-find alternative road
    //-divert traffic to alternative road -> * determine amount * update source * update destination
    // CHANGE_SIGNAL,
    //-check signal is green for which road
    //-check if signal is green for more than threshold time
    //-change signal to green for the most congested road -> * update source * update destination
    //test block road actions:
    // BLOCK_ROAD,
    //-find congested road
    //-find alternative road
    //-block all incoming traffic to congested road -> * update source * update destination
    //do nothing action:
    // Do Nothing Action,
    public:
    ActionType action;
    intersection* inter;
    edge* sourceRoad;
    edge* destinationRoad;
    int amount;
    actions(ActionType a){
        action=a;
    }


    //these are the most primitive actions that can be taken to reduce congestion, we will use these to go up the hierarchy to create more complex actions
    void blockroad(edge* e){
        e->blocked=true;
    }

    void diverttraffic(edge* source, edge* destination, int amount){
        source->traffic-=amount;
        destination->traffic+=amount;
    }
    void changesignal(intersection* i, edge* e){
        i->greenlight=e;
    }

    //sensory subtasks to check the state of the environment
    
    edge* findCongestedRoad(state* s){
        edge* congestedRoad=NULL;
        float maxCongestion=0;
        for(int i=0;i<s->g->intersections.size();i++){
            for(int j=0;j<s->g->intersections[i]->connections.size();j++){
                edge* e=s->g->intersections[i]->connections[j];
                float congestion=s->calculatecongestion(e);
                if(congestion>maxCongestion && !e->blocked){
                    maxCongestion=congestion;
                    congestedRoad=e;
                }
            }
        }
        return congestedRoad;
    }
    edge* findAlternativeRoad(state* s, edge* congestedRoad){
        edge* alternativeRoad=NULL;
        for(int i=0;i<s->g->intersections.size();i++){
            intersection* targetIntersection=NULL;
            for(int j=0;j<s->g->intersections[i]->connections.size();j++){
                edge* e=s->g->intersections[i]->connections[j];
                if(e==congestedRoad){
                    targetIntersection=s->g->intersections[i];
                    break;
                }
                else{
                    continue;
                }
            }
        float minCongestion=FLT_MAX;
        if(targetIntersection==NULL){
            continue;
        }
        for(int j=0;j<targetIntersection->connections.size();j++){
            edge* e=targetIntersection->connections[j];
            if(s->calculatecongestion(e)<minCongestion && !e->blocked){
                alternativeRoad=e;
                minCongestion=s->calculatecongestion(e);
            }
        }
        return alternativeRoad;
    }}

    // CHANGE_SIGNAL action:
    // 1. check which road currently has the green light
    // 2. check if it has stayed green longer than the threshold
    // 3. switch the green light to the most congested road at that intersection
    void changeSignalForCongestedRoad(state* s, int thresholdTime){
        for(int i=0;i<s->g->intersections.size();i++){
            intersection* currentIntersection=s->g->intersections[i];
            edge* currentGreen=currentIntersection->greenlight;
            edge* mostCongestedRoad=NULL;
            int maxTraffic=0;

            for(int j=0;j<currentIntersection->connections.size();j++){
                edge* e=currentIntersection->connections[j];
                if(!e->blocked && e->traffic > maxTraffic){
                    maxTraffic=e->traffic;
                    mostCongestedRoad=e;
                }
            }

            if(mostCongestedRoad==NULL){
                continue;
            }

            bool mustChangeSignal = (currentGreen==NULL || currentGreen!=mostCongestedRoad);

            if(!mustChangeSignal && currentIntersection->time >= thresholdTime){
                mustChangeSignal=true;
            }

            if(mustChangeSignal){
                currentIntersection->greenlight=mostCongestedRoad;
                currentIntersection->time=0;
            }
            else{
                currentIntersection->time++;
            }
        }
    }

    void reduceCongestion(state*s){
        edge* congestedRoad=findCongestedRoad(s);
        edge* alternativeRoad=findAlternativeRoad(s,congestedRoad);
        if(congestedRoad!=NULL && alternativeRoad!=NULL){
            int amount=congestedRoad->traffic/2;
            diverttraffic(congestedRoad,alternativeRoad,amount);
        }
    }
    
    //NOw lets do the block road action
    void blockCongestedRoad(state* s){
        edge* congestedRoad=findCongestedRoad(s);
        if(congestedRoad!=NULL){
            blockroad(congestedRoad);
        }
    }
    //this is the most basic action for block , i cant improve on it rn

};
class planner{
public:

    float maxCongestion = 0.8;
    int signalThreshold = 10;

    actions* actionHandler;

    planner(actions* a){
        actionHandler = a;
    }

    void plan(state* s){

        // Find whether congestion exists
        edge* congestedRoad =
            actionHandler->findCongestedRoad(s);

        if(congestedRoad == NULL){

            cout << "No congestion. DO NOTHING\n";

            return;
        }

        cout << "Congested road found.\n";

        // HTN METHOD 1:
        // DIVERT_TRAFFIC

        edge* alternativeRoad =
            actionHandler->findAlternativeRoad(
                s,
                congestedRoad
            );

        if(alternativeRoad != NULL){

            cout << "Executing DIVERT_TRAFFIC\n";

            actionHandler->reduceCongestion(s);

            return;
        }

        // HTN METHOD 2:
        // CHANGE_SIGNAL

        cout << "No alternative road.\n";
        cout << "Trying CHANGE_SIGNAL\n";

        actionHandler->changeSignalForCongestedRoad(
            s,
            signalThreshold
        );

        return;
    }
};

class sys{
    public:
    state* s;
    planner* p;
    sys(state* st, planner* pl){
        s=st;
        p=pl;
    }
    void run(){
        for (int i=0;i<5;i++){
            p->plan(s);
            //simulate traffic flow
            for(int i=0;i<s->g->intersections.size();i++){
                intersection* inter=s->g->intersections[i];
                if(inter->greenlight!=NULL){
                    inter->greenlight->traffic-=p->signalThreshold;
                    if(inter->greenlight->traffic<0){
                        inter->greenlight->traffic=0;
                    }
                }
            }
            s->displayState();
        }
    }

};

int main(){
    graph* g=new graph();
    intersection* i1=new intersection();
    intersection* i2=new intersection();
    intersection* i3=new intersection();
    g->addintersection(i1);
    g->addintersection(i2);
    g->addintersection(i3);
    edge* e1=new edge(10,0,1,20,false);
    edge* e2=new edge(5,0,2,20,false);
    edge* e3=new edge(15,1,2,20,false);
    g->addedge(e1);
    g->addedge(e2);
    g->addedge(e3);
    state* s=new state(g);
    actions* a=new actions(DO_NOTHING);
    planner* p=new planner(a);
    sys* syst=new sys(s,p);
    syst->run();
}
