#include <iostream>
#include <fstream>
#include<string>

using namespace std;
class File_reader{
    public : 
        string dir;
        string content="";
        explicit File_reader(string dir):dir(){
            
            string myText;

        // Read from the text file
        ifstream MyReadFile(dir);

        // Use a while loop together with the getline() function to read the file line by line
        while (getline(MyReadFile, myText)) {
        // Output the text from the file
            content += myText +" ";
        }

        // Close the file
MyReadFile.close();
        }
};