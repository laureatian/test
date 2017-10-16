class Memory{
public :
   int page;
   int read; // 0 for read, 1 for write.
   long address;
   Memory(int page, int read, long address){
   page = page;
   read = read;
   address = address;
   }
   ~Memory(){}
  };


