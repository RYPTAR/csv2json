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
class element{
    private:
        vector<string
}*/
class Column {
    public:
        size_t head;
        size_t tail;
        string data;
};
class Row {
    public:
        vector<Column> column;
        vector<size_t> comma;
        string line;

        /*void map(string line){
            vector<size_t> comma;
            for( int i = 0; i < line.length(); ++i){
                if(line[i] == ',')
                    comma.push_back(i);
            }
        }*/

};

// Locates commas
vector<size_t> MapCSV(string str){
    vector<size_t> comma;
    for( int i = 0; i < str.length(); ++i){
        if(str[i] == ',')
            comma.push_back(i);
    }
    return comma;
}
// Decodes the column rows
vector<Column> DecodeRow(string str, vector<size_t> map){
    vector<Column> column;
    Column temp;
    for( int i = 0; i < map.size(); ++i){

        // First element handler
        if(i == 0){
            temp.head = 0;
            temp.tail = map[i] - 0;
            temp.data = str.substr(temp.head, temp.tail);
            column.push_back(temp);
        }

        // Last element handler
        if( i == map.size() ){
            temp.head = map[i-1] + 1;
            temp.tail = str.length() - temp.head; 
        }

        // Middle element handler
        else{
            temp.head = map[i] + 1;
            // TODO: FIX THIS FUCKING TAIL!
            temp.tail = (map[i+1] - map[i]) - 1;
        }

        temp.data = str.substr(temp.head, temp.tail);
        column.push_back(temp);
    }
    return column;
}

int main(int argc, char* const argv[]){

    // Pulls in file
    ifstream file(argv[1]);
    string line;

    //TODO: multiple arguements

    // Decodes command
    string args = string(argv[2]);
    string command = args.substr(0,args.find('['));
    //cout<<command<<endl;


    if(command == "struct"){

        // Finds list of command arguements
        size_t length = args.find(']') - args.find('[');
        string subArgs = args.substr(args.find('[') + 1, length - 1);
        //cout<<subArgs<<endl;
        
        // Locates commas
        vector<size_t> comma = MapCSV(subArgs);
        //cout<<comma.size()<<endl;

        // Decodes the command arguements
        vector<Column> order = DecodeRow(subArgs, comma);
        /*for( int i = 0; i < comma.size(); ++i){
            if(i == 0)
                order.push_back(subArgs.substr(0, comma[i]));

            if( i == comma.size() ){
                order.push_back(subArgs.substr(comma[i-1] + 1));
            }
            else{
                size_t length = comma[i+1] - comma[i];
                order.push_back(subArgs.substr(comma[i] + 1, length - 1));   
            }
        }*/
        /*for( int i = 0; i < order.size(); ++i){
            cout<<"head= "<<order[i].head<<" tail= "<<order[i].tail<<" data= "<<order[i].data<<endl;
            //cout<<"data= "<<order[i].data<<endl;
        }*/

        /*vector<Column> csvMap;
        for( int i = 0; i < order.size(); ++i){

            //string header = getline(file,line);
            for( int i = 0; i < header.size(); ++i){
                if(i == 0)
                    order.push_back(subArgs.substr(0, comma[i]));

                if( i == comma.size() ){
                    order.push_back(subArgs.substr(comma[i-1] + 1));
                }
                else{
                    size_t length = comma[i+1] - comma[i];
                     order.push_back(subArgs.substr(comma[i] + 1, length - 1));   
                 }
            }*/
            /*while(getline(file, line)){
        
            }
            //data.push_back(element);
            file.clear();
            file.seekg(0, ios::beg);
        }*/
    }
}