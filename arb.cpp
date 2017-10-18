#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<math.h>
#include<set>
#include<queue>
#include<bitset>
#include<map>
#include<algorithm>
#include"memory.h"
#include"page.h"
using namespace std;
#define PAGESIZE 4096 
#define MEMORYSIZE 4
#define MAXPAGENUM 1000
#define INTERVAL  30
using BIT8 = bitset<8>;
typedef pair<int, int> PAIR;
long hextodecimal(string hex);
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

long hextodecimal(string hex){
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

int mycompare(const Page &p1, const Page &p2){

    return p1.refernum < p2.refernum;

}

int mapcompare(std::pair<int,int> ele1, std::pair<int,int>  ele2){
    return ele1.second < ele2.second;
}
struct CmpByValue {  
  bool operator()(const pair<int,int>& lhs, const pair<int,int>& rhs) {  
    return lhs.second < rhs.second;  
  }  
};  
int arb(string file)
{
    set<int> pageset;
    map<int,int>::iterator it;
  //  queue<int> memorypage;
    int iswrite[MAXPAGENUM] = {0};
    map<int,int> pagemap;
    vector<BIT8>  shift(MAXPAGENUM);
    int eventsnum = 0;
    int diskreads = 0;
    int diskwrites = 0;
    int prefetches = 0;
    char s[80];
    ifstream  fin;
    string line;
    int page;
    int  temp = 0; 
    fin.open("example1-3.trace",ios::in);
    while(fin.getline(s,80)){
   
        if(s[0] == 'W' || s[0] == 'R')
           eventsnum += 1;
        else 
           continue;
 
        line = s;
        line = line.substr(2,8);
        int address = hextodecimal(line);
        int page = address / PAGESIZE ;
//`        cout << "page " << page << endl;          
 
        cout <<"before set "<< shift[page] << endl;
        shift[page].set(7);
        cout <<"after set "<< shift[page] << endl;
        if(s[0] == 'W'){
            iswrite[page] = 1;
        }
//        it = pagemap.find(page);
        if(pagemap.find(page) == pagemap.end()){
            cout << "MISS:    "<<"page " << page <<endl;
            if(pagemap.size() == MEMORYSIZE){
               for(it = pagemap.begin(); it != pagemap.end(); it ++){
                   it->second = (int) (shift[it->first].to_ulong());
                   cout << "bitset " << shift[it->first] <<endl;
                   cout<<"elefirst "<< it->first <<"ele.second "<< it->second <<endl;
               }
              
                vector<PAIR> pagevector(pagemap.begin(),pagemap.end());
                sort(pagevector.begin(), pagevector.end(),CmpByValue());
                 
                for(int k = 0;  k < MEMORYSIZE; k++){
                 cout<< "vector[k]" << pagevector[k].first << "  " << pagevector[k].second << endl;

                } 
                
                int first = pagevector[MEMORYSIZE - 1].first;
                pagemap.erase(first);
                pageset.erase(first);
              
                if(iswrite[first] == 1){
                    diskwrites += 1;
                    iswrite[first] = 0;
                    cout << "REPLACE: "  << "page "<<first<< " (DIRTY)" << endl;
                } else {
                    cout << "REPLACE: "  <<"page "<< first <<  endl;
                }
            }
            pagemap[page] = 0;
//            memorypage.push(page);
            pageset.insert(page);
            diskreads += 1;
        } else {
         
          cout << "HIT:     " <<"page " << page << endl;
        }
    temp ++;
    if (temp == INTERVAL) {
       temp = 0;
       for (int  i = 0 ; i < shift.size(); i++){
            shift[i] >> 1;

       }
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
arb(s);
return 0;

}
