#include "./include/file_reader.hpp"

#include <iostream>
using namespace std;


int main(){

    File_reader a("test.txt");
    cout<<a.content<<endl;
    return 0;
    
}