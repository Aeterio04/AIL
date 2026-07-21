#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

class node{
    public:
    string city;
    int visited=0;
    pair<int,int> coordinates;
    vector<node*> conns;

    node(string c,pair<int,int> coords){
        city=c;
        coordinates=coords;
        conns={};
    }
    void connect(node* n){
        conns.push_back(n);
        n->connect(this);
    }

    int heuristic(node* a,node* b ){
        int xdistance=(a->coordinates.first-b->coordinates.first)*(a->coordinates.first-b->coordinates.first);
        int ydistance=(a->coordinates.second-b->coordinates.second)*(a->coordinates.second-b->coordinates.second);
        return xdistance+ydistance;
    }

    vector<node*> pathfinder(node* origin, node* destination,vector<node*> path){
        
        if(origin==destination){
            path.push_back(destination);
            return path;
        }

        for(int i=0;i<origin->conns.size();i++){
            if(origin->conns[i]->visited==0){
                path.push_back(origin);
                origin->visited=1;
                pathfinder(origin->conns[i],destination,path);
                if(path.back()==destination){
                    return path;
                }
                else{
                path.pop_back();
                origin->visited=0;
                }
            }
            else{
                continue;
            }
        }
        return path;
    }

    vector<node*> pathfinderPlus(node* origin,node* destination,vector<node*> path){
        node* min=origin;
        if(origin==destination){
            return path;
        }
        int h=heuristic(origin,destination);
        for(int i=0;i<origin->conns.size();i++){
            if(heuristic(origin->conns[i],destination)<h){
                min=origin->conns[i];
            }
        }
        if(min==origin){
            return path;
        }
        else{
            path.push_back(min);
            pathfinderPlus(min,destination,path);
        }
    }

};

void print(vector<node*> path){
    for(int i=0;i<path.size();i++){
        cout<<path[i]->city<<"  "<<path[i]->coordinates.first<<" "<<path[i]->coordinates.second<<endl;
    }
}


int main(){
    node* city1=new node("Mumbai",{0,0});
    node* city2=new node("Pune",{2,-1});
    node* city3=new node("Banglore",{8,-4});
    node* city4=new node("Delhi",{4,9});
    node* city5=new node("Indore",{6,2});
    node* city6=new node("Ahmedabad",{1,4});
    node* city7=new node("Chennai",{5,-9});
    
    city1->connect(city2);
    city2->connect(city3);
    city3->connect(city4);
    city3->connect(city7);
    city1->connect(city6);
    city6->connect(city4);
    city2->connect(city5);

    node* cityTemp;
    vector<node*> path={};
    path=cityTemp->pathfinder(city6,city5,path);
    print(path);
}

