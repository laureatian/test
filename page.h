#include<bitset>
//using std::bitset = bitset;
using namespace std;
class Page{
public:
int id;
int dirty; //0 for clean 1 for dirty
bitset<8> refer;
int refernum;
Page(int id ){
id = id;
}
~Page(){}
};
