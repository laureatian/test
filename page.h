#include<bitset>
//using std::bitset = bitset;
using namespace std;
class page{
public:
int id;
int dirty; //0 for clean 1 for dirty
bitset<8> refer;
int refernum;
page(int id, int read){
id = id;
dirty =read;
}
~page(){}
};
