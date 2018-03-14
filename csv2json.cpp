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
    //cout<<command<<endl;

    if(command == "struct"){
        size_t length = args.find(']') - args.find('[');
        string subArgs = args.substr(args.find('[') + 1, length - 1);
        //cout<<subArgs<<endl;

        vector<size_t> comma;
        for( int i = 0; i < subArgs.length(); ++i){
            if(subArgs[i] == ',')
                comma.push_back(i);
        }
        //cout<<comma.size()<<endl;
        vector<string> order;
        for( int i = 0; i < comma.size(); ++i){
            if(i == 0)
                order.push_back(subArgs.substr(0, comma[i]));

            if( i == comma.size() )
                order.push_back(subArgs.substr(comma[i-1] + 1));
            else{
                size_t length = comma[i+1] - comma[i];
                order.push_back(subArgs.substr(comma[i] + 1, length - 1));   
            }
        }
        /*for( int i = 0; i < order.size(); ++i){
            cout<<order[i]<<endl;
        }*/

        //while(getline(file, line)){
        

        
        //}
        //inputFile.clear();
        //inputFile.seekg(0, ios::beg);
    }
}