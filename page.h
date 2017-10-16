clas page{
int id;
int dirty; //0 for clean 1 for dirty
page(int id, int read):id(id); dirty(dirty){}
~page(){}
}
