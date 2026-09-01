#include <iostream>
#include <string>
#include <vector>
using namespace std;

class car{
    public:
    bool ev=false;
    bool disabled=false;
    bool large=false;
    int licenseplate;
    int registration;
    car(bool isev, bool isdis, bool islarge,int lplate){
        ev=isev;
        disabled=isdis;
        large=islarge;
        licenseplate=lplate;
    }
    

};
class slot{
    public:
    car* c=nullptr;
    bool ev=false;
    bool disabled=false;
    bool large=false;
    slot() : c(nullptr), ev(false), disabled(false), large(false) {}
};
class parkingSpace{
    public:
    

    void assign(slot* s1, bool isev, bool isdis, bool islarge ){
        s1->ev=isev;
        s1->disabled=isdis;
        s1->large=islarge;
    }

    car* defcar(){
        string ev, dis, large;
        int lplate;
        bool ev1,dis1,large1;
        cout<<"Is your car electric : "<<endl;
        cin>>ev;
        cout<<"Is your car length > 10m : "<<endl;
        cin>>large;
        cout<<"Are you disabled in any form : "<<endl;
        cin>>dis;
        cout<<"WHats the license plate? "<<endl;
        cin>>lplate;
        if(ev=="y" ){
            ev1=true;
        }
        if(dis=="y"){
            dis1=true;
        }
        if(large=="y"){
            large1=true;
        }
        car* c= new car(ev1,dis1,large1,lplate);
        return c;
    }
    bool insert(car* c1,slot* s1){
        if( s1->c!=NULL){
            return false;
        }
        else if(s1->disabled && !c1->disabled){
            return false;
        }
        else if(s1->ev && !c1->ev){
            return false;
        }
        else if(s1->large && !c1->large){
            return false;
        }
        else{
            s1->c=c1;
            return true;
        }
    }
    void specialslot(vector<vector<slot*>>& space, int row, int col){
        string ev, dis, large;
        bool ev1,dis1,large1;
        cout<<"Is the slot electric : "<<endl;
        cin>>ev;
        cout<<"Is the slot length > 10m : "<<endl;
        cin>>large;
        cout<<"Is the slot for the disabled : "<<endl;
        cin>>dis;
        if(ev=="y" ){
            ev1=true;
        }
        if(dis=="y"){
            dis1=true;
        }
        if(large=="y"){
            large1=true;
        }
        space[row][col]->disabled=dis1;
        space[row][col]->ev=ev1;
        space[row][col]->large=large1;
    }
    bool process( vector<car*> q,vector<vector<slot*>>& space){
        for (car* c : q) {
            slot* best = nullptr;
            int cscore=0;
            for (int i = 0; i < space.size(); i++) {
                for (int j = 0; j < space[i].size(); j++) {
                    slot* s = space[i][j];
                    int sscore=0;
                    if (s->c != nullptr) continue;
                    if (s->disabled && !c->disabled) continue;
                    if (s->ev && !c->ev) continue;
                    if (s->large && !c->large) continue;

                    if (best == nullptr) {
                        best = s;
                        if(s->disabled==best->disabled){
                            sscore++;
                        }
                        if(s->ev==best->ev){
                            sscore++;
                        }
                        if(s->large==best->large){
                            sscore++;
                        }
                         
                        cscore=sscore;
                    }
                    else {
                        bool sSpecial = s->disabled || s->ev || s->large;
                        bool bSpecial = best->disabled || best->ev || best->large;
                        if(s->disabled==best->disabled){
                            sscore++;
                        }
                        if(s->ev==best->ev){
                            sscore++;
                        }
                        if(s->large==best->large){
                            sscore++;
                        }
                            
                        if (sscore>cscore) {
                            best = s;
                        }
                    }
                }
            }
            if (best == nullptr) {
                return false;
            }
            best->c = c;
        }
        return true;
    }
    
    void start(){
        int i,j,num;
        cout<<"Enter size of lot :";
        cin>>i;
        cin>>j;
        cout<< "Enter Number of Cars :";
        cin>>num;

        vector<car*> q;
        for(int i=0;i<num;i++){
            q.push_back(defcar());
        }
        vector<vector<slot*>> space(j,vector<slot*>(i, nullptr));
        for(int r=0; r<space.size(); r++){
            for(int c=0; c<space[r].size(); c++){
                space[r][c] = new slot();
            }
        }
        char temp;
        int temp1,temp2;
        while (true){
            cout<<endl<<"are there any special slots :";
            cin>>temp;
            if(temp=='y'){

                cout<<"Slot row "<<endl;
                cin>>temp1;
                cout<<"Slot col"<<endl;
                cin>>temp2;
                specialslot(space,temp1,temp2);
            }
            else{break;}
        }

        process(q,space);
        showspace(space);

    }
    void showspace(vector<vector<slot*>> slot){
        for(int i=0;i<slot.size();i++){
            for(int j=0;j<slot[i].size();j++){
                if(slot[i][j]->c== NULL){
                    cout<< "None    ";
                }
                else{
                    cout<<slot[i][j]->c->licenseplate<<"    ";
                }
            }
            cout << endl;
        }
    }



};

int main(){
    parkingSpace p;
    p.start();    
}