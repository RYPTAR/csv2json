#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cctype>

using namespace std;

/*class JSONformat{
    private:
        string head;
        vector<string> objs;
        vector<string> column;
}
class elements{
    private:
        vector<string
}*/

int main(int argc, char* const argv[]){
    ifstream file(argv[1]);
    string line;
    //TODO: multiple arguements

    string args = string(argv[2]);
    string command = args.substr(0,args.find('['));

    if(command == "struct"){
        string subArgs = args.substr(args.find('[') + 1);
        subArgs.pop_back();
        //cout<<command<<endl;
        //cout<<order<<endl;

        int count = 0;
        for( int i = 0; i < subArgs.length(); ++i){
            if(subArgs[i] == ',')
                ++count;
        }
        cout<<count<<endl;
        //vector<string> order = new vector<string>;
        //for( int i = 0; i < count + 2){
            //order
        //for(
        //while(){

        //while(getline(file, line)){
        

        
        //}
        //inputFile.clear();
        //inputFile.seekg(0, ios::beg);
    }
}