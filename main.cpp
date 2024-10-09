#include <string>
#include <iostream>
#include <sstream>
#include "fileparce.h"
#include "structures.h"

using namespace std;

void commandhandler(string& filename, string& dquery);
void work_hash(string& word, stringstream& query, string& filename, string& command);
void work_list(string& word, stringstream& query, string& filename, string& command);
void work_stack(string& word, stringstream& query, string& filename, string& command);
void work_queue(string& word, stringstream& query, string& filename, string& command);
void work_array(string& word, stringstream& query, string& filename, string& command);
void work_tree(string& word, stringstream& query, string& filename, string& command);

int main(int argc, char* argv[]) {
    try{
        string file;
        string query;

        for (int i = 1; i < argc; ++i) {
            string arg = argv[i];

            if (arg == "--file" && i + 1 < argc) {
                file = argv[++i];
            } else if (arg == "--query" && i + 1 < argc) {
                query = argv[++i];
            }
        }

        commandhandler(file, query);
    }catch(runtime_error err){
        cerr << err.what() << endl;
    }

    return 0;
}

void commandhandler(string& filename, string& dquery){
    stringstream query;
    query << dquery;

    string word;
    string command;
    query >> command;

    if(command[0] == 'L'){
        work_list(word, query, filename, command);

    }else if(command[0] == 'H'){
        work_hash(word, query, filename, command);
    }else if (command[0] == 'S'){
        work_stack(word, query, filename, command);
    }else if (command[0] == 'Q'){
        work_queue(word, query, filename, command);
    }else if (command[0] == 'M'){
        work_array(word, query, filename, command);
    }else if (command[0] == 'T'){
        work_tree(word, query, filename, command);
    }

}


template <typename T>
void printList(T values){
    for(int i = 0; i < values.length; i++){
        cout << values[i] << ' ';
    }
}

void work_hash(string& word, stringstream& query, string& filename, string& command){
     query >> word;

    HashTable<string> res;
    List<string> keys;
    List<string> values;

    find_values_in_file(filename, word+".keys", keys);
    find_values_in_file(filename, word+".values", values);

    for(int i = 0; i < keys.length; i++){
        res.Add(keys[i], values[i]);
    }

    if (command == "HADD"){
        string key;
        string value;

        query >> key;

        query >> value;

        res.Add(key, value);
        keys.push(key);
        values.push(value);

        rewrite_values_in_file(filename, word+".keys", keys);
        rewrite_values_in_file(filename, word+".values", values);
    }else if (command == "HGET"){
        string key;
        query >> key;

        cout << res.Get(key) << endl;
        
    }else if (command == "HPRINT"){
        cout << "keys: " << endl;
        printList(keys);
        cout << endl;
        cout << "values: " << endl;
        printList(values);
    }else if (command == "HREMOVE"){
        string key;
        query >> key;

        res.Remove(key);

        int to_delete = keys.find(key);
        keys.del(to_delete);
        values.del(to_delete);

        rewrite_values_in_file(filename, word+".keys", keys);
        rewrite_values_in_file(filename, word+".values", values);
    }

    keys.clear();
    values.clear();

}

void work_list(string& word, stringstream& query, string& filename, string& command){
    List<string> res;
    query >> word;
    
    find_values_in_file(filename, word, res);
    if (command == "LFIND"){
        string value;
        query >> value;

        cout << res.find(value);
    }else if (command == "LPUSHF"){
        string value;
        query >> value;

        res.pushf(value);

        rewrite_values_in_file(filename, word, res);
    }else if (command == "LDEL"){
        int value;
        query >> value;

        res.del(value);

        rewrite_values_in_file(filename, word, res);
    }else if (command == "LPUSH"){
        string value;
        query >> value;

        res.push(value);

        rewrite_values_in_file(filename, word, res);
    }else if (command == "LGET"){
        int value;
        query >> value;

        cout << res[value];
    }else if (command == "LPRINT"){
        printList(res);
    }else if (command == "LREMOVE"){
        string value;
        query >> value;

        res.remove(value);

        rewrite_values_in_file(filename, word, res);
    }

    res.clear();
}

void work_stack(string& word, stringstream& query, string& filename, string& command){
    Stack<string> res;
    Stack<string> new_res;
    query >> word;

    find_values_in_file(filename, word, new_res);

    if (command == "SPUSH"){
        string value;
        query >> value;

        new_res.push(value);

        while (new_res.length != 0){
            res.push(new_res.pop());
        }

        rewrite_values_in_file_Odn(filename, word, res);
    }else if (command == "SPRINT"){
        while (new_res.length != 0){
            res.push(new_res.pop());
        }

        int o = res.length;

        for (int i = 0; i < o; i++){
            cout << res.pop() << ' ';
        }
        cout << endl;
    }else if (command == "SPOP"){
        cout << new_res.pop() << endl;

        while (new_res.length != 0){
            res.push(new_res.pop());
        }

        rewrite_values_in_file_Odn(filename, word, res);
    }

    res.clear();
}

void work_queue(string& word, stringstream& query, string& filename, string& command){
    Queue<string> res;
    query >> word;

    find_values_in_file(filename, word, res);

    if (command == "QPUSH"){
        string value;
        query >> value;
        res.push(value);

        rewrite_values_in_file_Odn(filename, word, res);
    }else if (command == "QPRINT"){
        int o = res.length;

        for (int i = 0; i < o; i++){
            cout << res.pop() << ' ';
        }
        cout << endl;
    }else if (command == "QPOP"){
        cout << res.pop() << endl;

        rewrite_values_in_file_Odn(filename, word, res);
    }

    res.clear();
}

void work_array(string& word, stringstream& query, string& filename, string& command){
    Array<string> res = Array<string>();
    query >> word;
    
    find_values_in_file_ARR(filename, word, res);
    if (command == "MFIND"){
        string value;
        query >> value;

        cout << res.find(value);
    }else if (command == "MPRINT"){
        for (int i = 0; i < res.length; i++){
            if (res.get(i).state){
                cout << res[i] << ' ';
            }
        }
        cout << endl;
    }else if (command == "MSET"){
        int index;
        query >> index;
        string value;
        query >> value;

        res.set(value, index);

        rewrite_values_in_file_ARR(filename, word, res);
    }else if (command == "MDEL"){
        int value;
        query >> value;

        res.del(value);

        rewrite_values_in_file_ARR(filename, word, res);
    }else if (command == "MPUSH"){
        string value;
        query >> value;
        res.push(value);

        rewrite_values_in_file_ARR(filename, word, res);
    }else if (command == "MGET"){
        printList(res);
    }else if (command == "MGETI"){
        int value;
        query >> value;

        cout << res[value] << endl;
    }else if (command == "MREMOVE"){
        string value;
        query >> value;

        res.remove(value);

        rewrite_values_in_file_ARR(filename, word, res);
    }
}

void work_tree(string& word, stringstream& query, string& filename, string& command){
    List<string> res;
    List<string> last_res;
    query >> word;

    find_values_in_file(filename, word, res);

    FullTree<string> resT;
    for(int i = 0; i < res.length; i++){
        resT.insert(res[i]);
    }

    res.clear();

    if (command == "TINSERT"){
        string value;
        query >> value;

        resT.insert(value);

        resT.get(last_res);

        rewrite_values_in_file(filename, word, last_res);
    }else if (command == "TCHECK"){
        cout << resT.checkFull() << endl;
    }else if (command == "TPRINT"){
        resT.print();
    }else if (command == "TFIND"){

    }

    resT.clear();
    last_res.clear();
}