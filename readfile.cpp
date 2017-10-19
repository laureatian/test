#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<math.h>
#include<set>
#include<queue>
#include"memory.h"
using namespace std;
#define PAGESIZE 512 
#define MEMORYSIZE 10
#define MAXPAGENUM 10000000




long long hextodecimal(string hex){
    long long sum = 0;
    for(int  i = 0; i < hex.length(); i++){
        if(hex[i] >= 'a' && hex[i] <= 'f'){
            sum += (hex[i]-87) * pow (16, hex.length() - 1 - i);
//            cout<< (hex[i]-87) * pow (16, 6 - i) << endl;
        } else{ 
            sum += atoi(hex.substr(i,1).data()) * pow (16, hex.length() - 1 - i);
  //          cout << atoi(hex.substr(i,1).data()) * pow (16, 6 - i) << endl;
        }
    }
return sum;
}


int fifo(string file)
{
    set<long> pageset;
    set<long>::iterator it;
    queue<long> memorypage;
    int iswrite[MAXPAGENUM] = {0}; 
    int eventsnum = 0;
    int diskreads = 0;
    int diskwrites = 0;
    int prefetches = 0;
    char s[80];
    ifstream  fin;
    string line;
    long page;
    
    fin.open("example3.trace",ios::in);
    while(fin.getline(s,80)){
   
        if(s[0] == 'W' || s[0] == 'R')
           eventsnum += 1;
        else 
           continue;
 
        line = s;
        cout << "line " << line << endl;
        int pos = line.find_first_of(" ");
        line = line.substr(pos + 1, line.length() - pos - 1);
        long long address  = hextodecimal(line);
        long page = address / PAGESIZE ;
        cout << "address " << address << endl;          
        cout << "page " << page << endl;          

        if(s[0] == 'W'){
            iswrite[page] = 1;
        }
        it = pageset.find(page);
        if(it == pageset.end()){
            cout << "MISS:    "<<"page " << page <<endl;
            if(pageset.size() == MEMORYSIZE){
                long first = memorypage.front();
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
