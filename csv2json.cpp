/***
 * 
 *            ---- csv2json.cpp ----
 *          
 *  created by: RYPTAR aka Ray Antonio De Jesus    
 *  date: 19/03/2018  
 * 
 *  description:
 *         This tool takes in csv data and tranlates the data into JSON.
 *         Formatting of the JSON data is defined by user arguments
 * 
 * 
 * 
***/
#include <fstream>
#include <string>
#include <vector>

using namespace std;

//*********** Objects **************

// Stores column data as well as the location of commas
class Column {
    public:
        size_t head;
        size_t tail;
        string data;
};
// Stores the JSON object hierarchy rules using column numbers(int)
class Rule {
    public:
        vector<int> rule;
};
// Stores the JSON object and relative JSON object information
class Obj {
    public:
        string name;
        vector<Obj> obj;
};

//*********** Functions **************

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
//Trims all whitespace
string trim(const string& str)
{
    // Checks for all whitespace delimitor
    size_t first = str.find_first_not_of(" \n\t\r\v\f");

    // Prevents empty data entries
    if (string::npos == first){
        return "------";
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
// Creates the JSON Structure Rules
//      i.e how the user wants to rearrange their JSON heirarchy
//          depending on the columns of their csv
vector<Rule> CreateRules(string str, vector<size_t> map){

    // Creates a order of data by decoded the row of data
    vector<Column> order = DecodeRow(str, map);
    vector<Rule> rules;

    // Depending on how many columns it scans through
    for(int i = 0; i < order.size(); ++i){

        // Length of the data element
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
// Organizes row columns into a vector with a JSON style hierarchy 
void buildTree(int i, vector<string> &column, const vector<Rule> &order, vector<Obj> &branch){

    // Check if there is any work left to do in order set
    if(i < order.size()){
        for(int j = 0; j < order[i].rule.size(); ++j){

            // Position of column
            int pos = order[i].rule[j];
            Obj temp;
            temp.name = column[pos];

            // Don't iterate is the branch is empty
            if(branch.size() < 1){
                branch.push_back(temp);
                pos = 0;
            }
            else{

                // Checks for unique values
                for(int k = 0; k < branch.size(); ++k){

                    // If value isn't unique, save position for 
                    //   the next level of the heirarchy
                    if(branch[k].name == temp.name){
                        pos = k;
                        break;
                    }

                    // Push if unique, save position for
                    //   the next level in heirarchy
                    else if(k == branch.size() - 1){
                        branch.push_back(temp);
                        pos = k+1;
                    }
                }
            }

            // Recalls function at lower levels of the tree
            buildTree(i+1, column, order, branch[pos].obj);
        }
    }   

    return;
}
// Prints JSON into a string
void outputJSON(int i, vector<Obj> &branch, string &result){

    // Formats the tabs based on how low in the tree you are
    string tabs;

    // Starting brace
    if( i == 0 )
        result+="{\n";
    for(int j = 0; j <= i; ++j){
        tabs+="  ";
    }

    // Starts at the top of the tree and searches down each branch
    for(int j = 0; j < branch.size(); ++j){

        // If a branch has children branches it recalls the outputJSON
        if(!branch[j].obj.empty()){
            result+=tabs+"\""+branch[j].name+"\":{\n";
            outputJSON(i+1, branch[j].obj, result);
            result+=tabs+"}";
        }
        else{
            result+=tabs+"\""+branch[j].name+"\":{}";
        }

        // If a branch has more sibling it adds a comma 
        if(j < branch.size() - 1)
            result+=",\n";
        else
            result+="\n";
    }

    // Ending brace
    if( i == 0 )
        result+="}\n";

    return;
}


//************************** MAIN **************************
int main(int argc, char* const argv[]){

    // Pulls in file
    ifstream file(argv[1]);
    string line;

    // TODO: custom output file names

    // TODO: multiple arguements

    // TODO: more JSON styling manipulation within struct arguements

    // Decodes command
    string args = string(argv[2]);
    string command = args.substr(0,args.find('['));

    // "struct" command line arguement
    if(command == "struct"){

        // Finds list of command arguements
        size_t length = args.find(']') - args.find('[');
        string subArgs = args.substr(args.find('[') + 1, length - 1);

        // Creates the JSON Structure Rules
        const vector<Rule> order = CreateRules(subArgs, MapSV(subArgs, ','));

        // Main "json" vector organized into JSON style tree
        vector<Obj> jsonTree;

        // Reusable row and column data stores
        vector<Column> row;
        vector<string> columnData;

        while(getline(file,line)){

            // Decodes the line with the coma positions
            row = DecodeRow(line, MapSV(line, ','));
            // Scrubs out the redundant data within the column objects
            columnData = SanitizeRow(row);
            
            // Start the JSON building process
            buildTree(0, columnData, order, jsonTree);
        }

        file.close();

        // Creates the JSON file
        ofstream jsonFile;
        jsonFile.open("data.json");

        // JSON file text
        string result;
        outputJSON(0,jsonTree,result);

        // Writes to JSON file
        jsonFile << result;
        jsonFile.close();
    }
}