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
#define MEMORYSIZE 10 
#define MAXPAGENUM 10000000
#define INTERVAL 4
#define WINDOWSIZE 4
#define DEBUG 1
using BIT8 = bitset<8>;
typedef pair<long, int> PAIR;

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

int mycompare(const Page &p1, const Page &p2){

    return p1.refernum > p2.refernum;

}

struct CmpByValue {  
  bool operator()(const pair<long,int>& lhs, const pair<long,int>& rhs) {  
    return lhs.second < rhs.second;  
  }  
};  
int workingset(string file)
{
    set<long> pageset;
    bitset<MAXPAGENUM> iswrite;
    vector<BIT8>  shift(MAXPAGENUM);
    vector<pair<long,int>>  pagevector;
    int eventsnum = 0;
    int diskreads = 0;
    int diskwrites = 0;
    int prefetches = 0;
    char s[80];
    ifstream  fin;
    string line;
    long page;
    int  temp = 0;
    int count = 0;
    int context_switch; 
    fin.open("example3.trace",ios::in);
    while(fin.getline(s,80)){
        count ++;
        if(s[0] == 'W' || s[0] == 'R')
           eventsnum += 1;
        else if(s[0] == '#' && (count != 1)){
            iswrite.reset();
            cout<<"before clear shift size " <<shift.size() <<endl;
            shift.clear();
            cout<<"after clear shift size " <<shift.size() <<endl;

            vector<pair<long,int>>().swap(pagevector);
            
            pageset.clear();
            cout <<"pagesetsize "<<pageset.size()<< endl;
            cout <<"pagevectorsize "<<pagevector.size()<< endl;
           for(int  i = 0; i < MEMORYSIZE; i++){
                fin.getline(s,80);
                eventsnum ++;
                line = s;
                cout <<"line" << line << endl;
                cout << "count " <<count <<endl;
                int pos = line.find_first_of(" ");
                line = line.substr(pos + 1,line.length() - pos -1 );
                long long address = hextodecimal(line);
                long page = address / PAGESIZE ;
     
                shift[page].set(7);
                if(s[0] == 'W'){
                    iswrite.set(page);
                 }
                 if(pageset.find(page) == pageset.end()){     
                     pagevector.push_back(pair<long,int>(page,0));
                     pageset.insert(page);
                     prefetches ++;
                     diskreads ++;
                 }
                 temp ++;
                 if (temp % INTERVAL == 0) {
                 for (int i = 0 ; i < shift.size(); i++){
                     shift[i] = shift[i] >> 1;
                 }
                 }
                 diskreads --;
            }
            continue;
        } else  
           continue;
 
        line = s;
        cout <<"line" << line << endl;
        cout << "count " <<count <<endl;
        int pos = line.find_first_of(" ");
        line = line.substr(pos + 1,line.length() - pos -1 );
        long long address = hextodecimal(line);
        long page = address / PAGESIZE ;
    //    cout << "address" << address << endl; 
      //  cout << "page" << page << endl; 
        shift[page].set(7);
        if(s[0] == 'W'){
            iswrite.set(page);
        }
        if(pageset.find(page) == pageset.end()){
            cout << "MISS:    "<<"page " << page <<endl;
            //cout << " mapsize:" <<  pagemap.size()<< endl; 
            if(pageset.size() == MEMORYSIZE){
                for(int i = 0 ; i < MEMORYSIZE; i++){
                 pagevector[i].second = (int) (shift[pagevector[i].first].to_ulong());


                } 
                sort(pagevector.begin(), pagevector.end(),CmpByValue());
                
                long first = pagevector[0].first;
                pagevector.erase(pagevector.begin());
                pageset.erase(first);
              
                if(iswrite[first] == 1){
                    diskwrites += 1;
                    iswrite.reset(first);
                    # if DEBUG
                    cout << "REPLACE: "  << "page "<<first<< " (DIRTY)" << endl;
                    # endif
                } else {
                    # if DEBUG
                    cout << "REPLACE: "  <<"page "<< first <<  endl;
                    # endif 
                }
            }
            pagevector.push_back(pair<long,int>(page,0));
            pageset.insert(page);
            diskreads += 1;
        } else {
            # if DEBUG
            cout << "HIT:     " <<"page " << page << endl;
            # endif
        }
        temp ++;
        if (temp == INTERVAL) {
            temp = 0;
            for (int i = 0 ; i < shift.size(); i++){
                shift[i] = shift[i] >> 1;
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
workingset(s);
return 0;

}
