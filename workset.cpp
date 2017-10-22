#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<math.h>
#include<set>
#include<queue>
#include<bitset>
#include<map>
#include<string>
#include<algorithm>
#include"memory.h"
#include"page.h"
using namespace std;
#define PAGESIZE 4096 
#define MEMORYSIZE 10 
#define MAXPAGENUM 10000000
#define INTERVAL 4
#define WINDOWSIZE 4
#define DEBUG 0
using BIT8 = bitset<8>;
typedef pair<long, int> PAIR;
using MEMVECTOR = pair<string,vector<pair<long,int>>>;

long long hextodecimal(string hex){
    long long sum = 0;
    for(int  i = 0; i < hex.length(); i++){
        if(hex[i] >= 'a' && hex[i] <= 'f'){
            sum += (hex[i]-87) * pow (16, hex.length() - 1 - i);
        } else{ 
            sum += atoi(hex.substr(i,1).data()) * pow (16, hex.length() - 1 - i);
        }
    }
return sum;
}

std::string& trim(std::string &s)   
{  
    if (s.empty())   
    {  
        return s;  
    }  
  
    s.erase(0,s.find_first_not_of(" "));  
    s.erase(s.find_last_not_of(" ") + 1);  
    return s;  
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
    vector<MEMVECTOR> memoryvector;
    set<string> memoryset;
    int eventsnum = 0;
    int diskreads = 0;
    int diskwrites = 0;
    int prefetches = 0;
    int pagefaults = 0;
    char s[80];
    ifstream  fin;
    string line;
    long page;
    int  temp = 0;
    int count = 0;
    int context_switch = 0;
    int processnum = MEMORYSIZE / WINDOWSIZE;
    string lastprocessname; 
    string processname;
    string addresstring;
    iswrite.reset();
    shift.clear(); 
    fin.open("example3.trace",ios::in);
    while(fin.getline(s,80)){
        count ++;
        line = s;
        line = trim(line);
        //cout <<"count " <<count <<endl;
        //cout <<"line " <<line <<endl;
        if(s[0] == 'W' || s[0] == 'R')
            eventsnum += 1;
        else if (s[0] == '#'){
            context_switch ++;
            //cout << "context switch  "<< context_switch << endl;
            int pos1 = line.find_last_of(" ");
            lastprocessname = processname;
            processname = line.substr(pos1 + 1,line.length() - pos1 -1);
            //cout << " processname length "<<processname.length() <<endl;
            //cout <<"process name                                " << processname << " count " << count<< endl;
            
            if (count != 1){
                /*cout <<"memorysetprint  size "<< memoryset.size()<< endl;
                cout << "                     ";
                for (set<string>::iterator it = memoryset.begin(); it != memoryset.end(); it ++ ){
                   cout << *it << " ";
                }
                cout << endl;*/

                // store pagevector to memoryvector && memoryset
                vector<PAIR> tempvector(pagevector);    
                memoryvector.push_back(MEMVECTOR(lastprocessname,tempvector));                 
                memoryset.insert(lastprocessname);
                //cout <<"memory vector size(after add)"<< memoryvector.size() << endl;
                     
                // after store, clear them
                pagevector.clear();
                pageset.clear();
           
                // if memory has this process, load it to pagevector
                if (memoryset.find(processname) != memoryset.end()){
                 //   cout << processname <<" in memory" << endl; 
                    for(int i = 0; i < memoryvector.size(); i++){
                        if (memoryvector[i].first == processname ){
             
                            vector<PAIR>  tempv = memoryvector[i].second;
                            for(int j = 0; j < tempv.size(); j++){
                                pageset.insert(tempv[j].first);
                            }
                            pagevector.assign(tempv.begin(),tempv.end());                    
                            memoryvector.erase(memoryvector.begin() + i); 
                            memoryset.erase(processname);       
                            break;
                        }
                    }
                 // delete from memoryvector && memoryset after load
                } else{ //  bu cun zai then prefetch
                    if (memoryvector.size() == (processnum + 1)){
                    
                        vector<PAIR>  vectortodelete = memoryvector[0].second; 
                        for (int k = 0; k < vectortodelete.size(); k ++){
                            if (iswrite[vectortodelete[k].first] ){
                                diskwrites ++;
                                iswrite.reset(vectortodelete[k].first);
                            }
                        }
                        memoryset.erase(memoryvector[0].first);
                        memoryvector.erase(memoryvector.begin());
                    } 
                    for(int  i = 0; i < WINDOWSIZE; i++){
           /*  label1: */   fin.getline(s,80);
                        count ++;
                        eventsnum ++;
                        line = s;
                        line = trim(line);
                        // cout << "line " <<line << endl;
                        //cout << "count " << count << endl;
                        int pos = line.find_first_of(" ");
                        addresstring = line.substr(pos + 1,line.length() - pos -1 );
                        long long address = hextodecimal(addresstring);
                        long page = address / PAGESIZE ;
      
                        if (s[0] == 'W'){
                            iswrite.set(page);
                        }
                        if (pageset.find(page) == pageset.end()){     
                            pagevector.push_back(pair<long,int>(page,0));
                            pageset.insert(page);
                            diskreads ++;
                            prefetches ++;
                        }
                        /*shift[page].set(7);
                        temp ++;
                        if (temp == INTERVAL){
                            temp = 0;
                            for (int l = 0; l < shift.size(); l ++){
                               shift[l] = shift[l] >> 1;

                            }
                        } */
                    }
                }  
            }
            continue;
        } else  
            continue;
         
        int pos = line.find_first_of(" ");
        addresstring = line.substr(pos + 1,line.length() - pos -1 );
        long long address = hextodecimal(addresstring);
        long page = address / PAGESIZE ;
        //    cout << "address" << address << endl; 
        //  cout << "page" << page << endl; 
        shift[page].set(7);
        if(s[0] == 'W'){
            iswrite.set(page);
        }
        if(pageset.find(page) == pageset.end()){
            #if DEBUG
            cout << "MISS:    "<<"page " << page <<endl;
            # endif
            //cout << " mapsize:" <<  pagemap.size()<< endl; 
            if(pageset.size() == MEMORYSIZE ){
                for(int i = 0 ; i < MEMORYSIZE; i++){
                 pagevector[i].second = (int) (shift[pagevector[i].first].to_ulong());
                } 
                sort(pagevector.begin(), pagevector.end(),CmpByValue());
                
                long first = pagevector[0].first;
                pagevector.erase(pagevector.begin());
                pageset.erase(first);
              
                if(iswrite[first]){
                    diskwrites ++;
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
            diskreads ++;
            pagefaults ++;
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
    cout << "page faults:        " << pagefaults << endl;
    cout << "prefetch faults:    " << prefetches << endl;
//    cout << "context switch      " << context_switch << endl;
    return 0;
}



int main(){
string s="00000abc";
workingset(s);
return 0;

}
