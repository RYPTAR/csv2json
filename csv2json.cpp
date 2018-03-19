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
/*class Row {
    public:
        vector<Column> column;
        vector<size_t> comma;
        string line;
};*/
class Rule {
    public:
        vector<int> rule;
};
class Obj {
    public:
        string name;
        vector<Obj> obj;
};

// Locates delimitors within a string 
//      i.e csv row
vector<size_t> MapSV(string str, char delimitor){
    vector<size_t> comma;
    for( int i = 0; i < str.length(); ++i){
        if(str[i] == delimitor)
            comma.push_back(i);
    }
    return comma;
}
// Isolates elements using a map of delimitors within a string 
///     i.e csv columns
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
        if( i == map.size() - 1 ){
            temp.head = map[i] + 1;
            temp.tail = str.length() - temp.head; 
        }

        // Middle elements handler
        else{
            temp.head = map[i] + 1;
            temp.tail = (map[i+1] - map[i]) - 1;
        }

        temp.data = str.substr(temp.head, temp.tail);
        column.push_back(temp);
    }
    return column;
}
//Trims whitespace
string trim(const string& str)
{
    size_t first = str.find_first_not_of(" \n\t\r\v\f");
    if (string::npos == first){
        return "NONE";
    }
    size_t last = str.find_last_not_of(" \n\t\r\v\f");
    return str.substr(first, (last - first + 1));
}
// Turns a decoded row into a list of string
vector<string> SanitizeRow(vector<Column> row){
    vector<string> list;
    for(int i = 0; i < row.size(); ++i){
        list.push_back(trim(row[i].data));
    }
    return list;
}
// 3AM FIRE! Lulz
// Creates the JSON Structure Rules
//      i.e how the user wants to rearrange their JSON heirarchy
//          depending on the columns of their csv
vector<Rule> CreateRules(string str, vector<size_t> map){

    // Creates a order of data by decoded the row of data
    vector<Column> order = DecodeRow(str, map);
    vector<Rule> rules;

    // Depending on how many columns it scans through
    for(int i = 0; i < order.size(); ++i){

        //length of the data element
        string line = order[i].data;
        Rule tempR;

        if(line.length() > 1){

            // Creates a temp vector column that decodes a line with the mapped SV data
            //      using '+' as the delimitor 
            vector <Column> tempC = DecodeRow(line, MapSV(line, '+'));

            // Depending on how many sub column there were rebuild
            for(int j = 0; j < tempC.size(); ++j){
                tempR.rule.push_back(stoi(tempC[j].data));
            }
        }
        else
            tempR.rule.push_back(stoi(line));

        rules.push_back(tempR);

    }

    return rules;
}
// Organizes rows columns into a vector with a JSON style hierarchy 
void buildTree(int i, vector<string> &column, const vector<Rule> &order, vector<Obj> &branch){

    //cout<<"---------------------"<<endl;
    // Check if there is any work left to do in order set
    if(i < order.size()){
        for(int j = 0; j < order[i].rule.size(); ++j){
            /*cout<<"i: "<<i<<" j: "<<j<<endl;
            cout<<"sizeO: "<<order.size()<<" sizeR: "<<order[i].rule.size()<<endl;
            cout<<"************************"<<endl;*/

            // Position of column
            int pos = order[i].rule[j];
            Obj temp;
            temp.name = column[pos];
            /*if(temp.name == "")
                temp.name = "NO ST NUM";*/
            //cout<<temp.name.length()<<" "<<temp.name<<endl;

            // Don't iterate is the branch is empty
            if(branch.size() < 1){
                /*cout<<"PUSH 1"<<endl;
                cout<<"level= "<<i<<" data= "<<temp.name<<endl;*/
                branch.push_back(temp);
                pos = 0;
            }
            else{

                // Checks for unique values
                for(int k = 0; k < branch.size(); ++k){

                    // If value isn't unique, save position for 
                    //   the next level of the heirarchy
                    if(branch[k].name == temp.name){
                        //cout<<"BREAK!"<<endl;
                        pos = k;
                        break;
                    }

                    // Push if unique, save position for
                    //   the next level in heirarchy
                    else if(k == branch.size() - 1){
                        /*cout<<"PUSH 2"<<endl;
                        cout<<"level= "<<i<<" data= "<<temp.name<<endl;*/
                        branch.push_back(temp);
                        pos = k+1;
                    }
                }
            }

            buildTree(i+1, column, order, branch[pos].obj);
        }
    }   

    return;
}
// Prints JSON
/*void coutJSON(int i, vector<Obj> &branch){
    string tabs;
    if( i == 0 )
        cout<<"{"<<endl;
    for(int j = 0; j <= i; ++j){
        tabs+="  ";
    }

    for(int j = 0; j < branch.size(); ++j){
        //cout<<branch[j].name<<endl;
        //cout<<tabs<<"\""<<branch[j].name<<"\":{"<<endl;

        if(!branch[j].obj.empty()){
            cout<<tabs<<"\""<<branch[j].name<<"\":{"<<endl;
            outputJSON(i+1, branch[j].obj);
            cout<<tabs<<"}";
        }
        else{
            cout<<tabs<<"\""<<branch[j].name<<"\":{}";
        }


        if(j < branch.size() - 1)
            cout<<","<<endl;
        else
            cout<<endl;
    }
    if( i == 0 )
        cout<<"}"<<endl;

    return;
}*/

void outputJSON(int i, vector<Obj> &branch, string &result){
    string tabs;
    if( i == 0 )
        result+="{\n";
    for(int j = 0; j <= i; ++j){
        tabs+="  ";
    }

    for(int j = 0; j < branch.size(); ++j){
        //cout<<branch[j].name<<endl;
        //cout<<tabs<<"\""<<branch[j].name<<"\":{"<<endl;

        if(!branch[j].obj.empty()){
            result+=tabs+"\""+branch[j].name+"\":{\n";
            outputJSON(i+1, branch[j].obj, result);
            result+=tabs+"}";
        }
        else{
            result+=tabs+"\""+branch[j].name+"\":{}";
        }


        if(j < branch.size() - 1)
            result+=",\n";
        else
            result+="\n";
    }
    if( i == 0 )
        result+="}\n";

    return;
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
        //vector<size_t> comma = MapSV(subArgs, ',');
        //cout<<comma.size()<<endl;

        // Decodes the command arguements
        /*vector<Column> order = DecodeRow(subArgs, comma);
        for( int i = 0; i < order.size(); ++i){
            cout<<"head= "<<order[i].head<<" tail= "<<order[i].tail<<" data= "<<order[i].data<<endl;
            //cout<<"data= "<<order[i].data<<endl;
        }*/

        // Creates the JSON Structure Rules
        const vector<Rule> order = CreateRules(subArgs, MapSV(subArgs, ','));
        /*for( int i = 0; i < order.size(); ++i){
            for( int j = 0; j < order[i].rule.size(); ++j){
                cout<<"i: "<<i<<" j: "<<j<<" size: "<<order[i].rule.size()<<" value: "<<order[i].rule[j]<<endl;
            }
        }*/
        vector<Obj> jsonTree;
        vector<Column> row;
        vector<string> columnData;
        int i = 1;
        while(getline(file,line) && i < 50){
            //cout<<"While"<<endl;
            row = DecodeRow(line, MapSV(line, ','));
            columnData = SanitizeRow(row);
            
            // Start the JSON building process
            //cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
            buildTree(0, columnData, order, jsonTree);
            //break;
            i++;
        }

        file.close();
        ofstream jsonFile;
        jsonFile.open("data.json");
        //cout<<"Tree: "<<jsonTree.size()<<endl;
        string result;
        outputJSON(0,jsonTree,result);
        //cout<<result;
        jsonFile << result;
        jsonFile.close();
    }
}