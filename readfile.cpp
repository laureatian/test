#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<math.h>
#include<set>
#include<queue>
#include"memory.h"
using namespace std;
#define PAGESIZE 4096 
#define MEMORYSIZE 4
#define MAXPAGENUM 1000



int hextodecimal(string hex);
int readfile(string file){
char s[80];
ifstream  fin;
string line;
int page;
fin.open("example1-3.trace",ios::in);
while(fin.getline(s,80)){

//if(s[0] == '#'){
//}
line.copy(s,8,2);
int address  = hextodecimal(line);
int page = address / PAGESIZE ;
if(s[0] == 'R'){
;
}else{
;
}
cout<<s<<endl;
}

return 0;
}

int hextodecimal(string hex){
    long sum = 0;
    if(hex.length() != 7) 
        return -1;
    for(int  i = 0; i < 7; i++){
        if(hex[i] >= 'a' && hex[i] <= 'f'){
            sum += (hex[i]-87) * pow (16, 6 - i);
//            cout<< (hex[i]-87) * pow (16, 6 - i) << endl;
        } else{ 
            sum += atoi(hex.substr(i,1).data()) * pow (16, 6 - i);
  //          cout << atoi(hex.substr(i,1).data()) * pow (16, 6 - i) << endl;
        }
    }
return sum;
}


int fifo(string file)
{
    set<int> pageset;
    set<int>::iterator it;
    queue<int> memorypage;
    int iswrite[MAXPAGENUM] = {0}; 
    int eventsnum = 0;
    int diskreads = 0;
    int diskwrites = 0;
    int prefetches = 0;
    char s[80];
    ifstream  fin;
    string line;
    int page;
    
    fin.open("example1-3.trace",ios::in);
    while(fin.getline(s,80)){
   
        if(s[0] == 'W' || s[0] == 'R')
           eventsnum += 1;
        else 
           continue;
 
        line = s;
        line = line.substr(2,8);
        int address  = hextodecimal(line);
        int page = address / PAGESIZE ;
//`        cout << "page " << page << endl;          

        if(s[0] == 'W'){
            iswrite[page] = 1;
        }
        it = pageset.find(page);
        if(it == pageset.end()){
            cout << "MISS:    "<<"page " << page <<endl;
            if(pageset.size() == MEMORYSIZE){
                int first = memorypage.front();
                pageset.erase(first);
                memorypage.pop();
                if(iswrite[first] == 1){
                    diskwrites += 1;
                    iswrite[first] = 0;
                    cout << "REPLACE: "  << "page "<<first<< " (DIRTY)" << endl;
                } else {
                    cout << "REPLACE: "  <<"page "<< first <<  endl;
                }
            }
            memorypage.push(page);
            pageset.insert(page);
            diskreads += 1;
        } else {
          cout << "HIT:     " <<"page " << page << endl;
        }
    }  
    cout << "events in trace:    " << eventsnum << endl;
    cout << "total disk reads:   " << diskreads << endl;
    cout << "total disk writes:  " << diskwrites << endl;
    cout << "page faults:        " << diskreads << endl;
    cout << "prefetch faults:    " << prefetches << endl;
    return 0;
}



int main(){
string s="00000abc";
fifo(s);
return 0;

}
