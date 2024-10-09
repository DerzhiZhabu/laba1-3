#include <string>
#include <iomanip>
#include <cmath>
#include <type_traits>

using namespace std;

template <typename T>
struct NodeD{
    T data;
    NodeD* next = nullptr;
    NodeD* prev = nullptr;
};

template <typename T>
struct NodeS{
    T data;
    NodeS* next = nullptr;
};

template <typename T>
struct Stack{
    int length = 0;
    NodeS<T>* tail = nullptr;

    void push(T data){
        NodeS<T>* new_node = new NodeS<T>{data, tail};
        tail = new_node;
        length += 1;
    }

    T pop(){
        if (length == 0) throw runtime_error("Stack is empty");

        NodeS<T>* new_node = tail->next;
        length -= 1;

        T value = tail->data;
        
        delete tail;
        tail = new_node;
        return value;
    }

    void clear(){
        while (length != 0){
            pop();
        }
    }
};

template <typename T>
struct Queue{
    int length = 0;
    NodeS<T>* head = new NodeS<T>;

    void push(T data){
        NodeS<T>* new_node = head;
        while((new_node -> next) != nullptr){
            new_node = new_node -> next;
        }
        new_node->next = new NodeS<T>{data, nullptr};
        length += 1;
    }

    T pop(){
        if (length == 0) throw runtime_error("Stack is empty");

        NodeS<T>* new_node = head->next;
        head->next = new_node->next;
        length -= 1;

        T value = new_node->data;
        
        delete new_node;
        return value;
    }

    void clear(){
        while (length != 0){
            pop();
        }
    }
};

template <typename T>
struct List{
    int length = 0;
    NodeD<T>* head = new NodeD<T>;
    NodeD<T>* tail = head;

    void push(T data){
        NodeD<T>* new_node = new NodeD<T>{data, nullptr, tail};
        tail -> next = new_node;
        tail = new_node;
        length += 1;
    }

    void pushf(T data){
        insert(data, 0);
    }

    T operator[](int index){
        return get(index);
    }

    T pop(){
        if (length == 0) throw runtime_error("List is empty");

        NodeD<T>* last = tail;

        T data = last -> data;
        tail -> prev -> next = nullptr;
        tail = tail -> prev;

        length --;

        delete last;
        return data;
    }

    T get(int index){
        if (index >= length || index < 0) throw runtime_error("Out of range");

        NodeD<T>* tek = head -> next;
        while (index != 0){
            tek = tek -> next;
            index --;
        }
        return tek -> data;
    }

    int find(T data){
        
        for (int index = 0; index < length; index++){
            if (get(index) == data){
                return index;
            }
        }

        return -1;
    }

    void del(int index){
        if (index >= length || index < 0) throw runtime_error("Out of range");
        if (index == length - 1) pop();

        NodeD<T>* tek = head -> next;
        while (index != 0){
            tek = tek -> next;
            index --;
        }

        NodeD<T>* pred = tek -> prev;
        pred -> next = tek -> next;
        pred -> next -> prev = pred;

        delete tek;
        length --;

    }

    void remove(T data){
        int index = find(data);

        if(index == -1) throw runtime_error("No value");

        del(index);
    }

    void insert(T data, int index){
        if (index > length || index < 0) throw runtime_error("Out of range");

        if (index == length){
            push(data);
        }

        else if (index >= 0){
            NodeD<T>* new_node = new NodeD<T>;
            new_node -> data = data;
            NodeD<T>* tek = head -> next;

            while (index != 0){
                tek = tek -> next;
                index --;
            }

            new_node -> prev = tek -> prev;
            tek -> prev -> next = new_node;
            tek -> prev = new_node;
            new_node -> next = tek;
            length++;
        }

    }

    void clear(){
        NodeD<T>* prev;
        NodeD<T>* tek = tail;
        while (tek != head){
            prev = tek ->prev;
            delete tek;
            tek = prev;
        }
        delete tek;
    }
};


template <typename T>
struct Array{

    struct Node{
        T value;
        bool state = false;
    };

    int next_push = 0;
    int size = 8;
    int length = size;
    int last_used = 0;
    Node* head = new Node[size];

    void resize(){
        Node* old_arr = head;
        head = new Node[size * 2];
        for(int i = 0; i < size; i++){
            if (old_arr[i].state){
                head[i] = old_arr[i];
            }
        }
        size *=2;
        length = size;
    }

    void check(){
        if (next_push == size){
            resize();
        }
    }

    void push(T data){
        head[next_push].value = data;
        head[next_push].state = true;
        next_push++;
        check();
    }

    void set(T data, int index){
        if (index >= length) throw runtime_error("Out of range");
        head[index].value = data;
        head[index].state = true;
        if(index >= next_push) next_push = index + 1;
        check();
    }

    Node get(int index){
        if (index >= length) throw runtime_error("Out of range");

        return head[index];
    }

    T operator[](int index){
        if (index >= length) throw runtime_error("Out of range");
        if (!head[index].state) throw runtime_error("No value");
        return head[index].value;
    }

    void del(int index){
        if (index >= length) throw runtime_error("Out of range");
        if (!head[index].state) throw runtime_error("No value");
        head[index].state = false;
    }

    int find(T data){
        for (int i = 0; i < length; i++){
            if(head[i].value == data && head[i].state){
                return i;
            }
        }

        return -1;
    }

    void remove(T data){
        int index = find(data);
        if  (index == -1) throw runtime_error("No value");

        head[index].state = false;
    }

    ~Array(){
        delete[] head;
    }

};

template <typename T>
struct NodeH{
    string key;
    T value;
    bool state = false;
    bool deleted = false;
};


template <typename T>
struct HashTable{


    int size = 8;

    NodeH<T>* arr = new NodeH<T>[size];

    ~HashTable(){
        delete [] arr;
    }

    int hashFunc(string key){
        int hash_result = 0;
        for (int i = 0; i < key.size(); i++){
            hash_result = ((size - 1) * hash_result + key[i]) % size;
            hash_result = (hash_result * 2 + 1) % size;
        }
        return hash_result;
    }

    void Add(string key, T value){
        int index = hashFunc(key);
        
        while (index < size){
            if (!arr[index].state){
                arr[index].key = key;
                arr[index].value = value;
                arr[index].state = true;
                return;
            }
            else if(arr[index].key == key){
                throw runtime_error("Key exists");
            }
            index++;
        }

        Resize();
        Add(key, value);
    }

    void Resize(){
        size *= 2;
        NodeH<T>* oldArr = arr;

        arr = new NodeH<T>[size];

        for(int i = 0; i < size / 2; i++){
            if (oldArr[i].state && !oldArr[i].deleted){
                Add(oldArr[i].key, oldArr[i].value);
            }
        }

        delete[] oldArr;
    }

    T Get(string key){
        int index = hashFunc(key);
        
        while (index < size){
            if (arr[index].state && arr[index].key == key){
                return arr[index].value;
            }
            else if (arr[index].key == key && arr[index].deleted) throw runtime_error("No such key");
            else if (!arr[index].deleted && !arr[index].state) throw runtime_error("No such key");
            index++;
        }

        throw runtime_error("No such key");
    }

    void Remove(string key){
        int index = hashFunc(key);
        while (index < size){
            if (arr[index].state && arr[index].key == key){
                if (is_same<T, List<string>>::value) arr[index].value.clear();
                arr[index].state = false;
                arr[index].deleted = true;
                return;
            }
            else if (arr[index].key == key && arr[index].deleted) throw runtime_error("No such key");
            else if (!arr[index].deleted && !arr[index].state) throw runtime_error("No such key");
            index++;
        }
        throw runtime_error("No such key");
    }
};

template<typename T>
struct FullTree{

    struct NodeT{
        T data;
        NodeT* left = nullptr;
        NodeT* right = nullptr;
    };

    NodeT* root = nullptr;


    void insert(T value){
        if (root == nullptr) {
            root = new NodeT{value};
        } else {
            insertHelper(root, value);
        }
    }

    void insertHelper(NodeT* node, T& value){
        Queue<NodeT*> q;
        q.push(node);
        while(q.length != 0){
            NodeT* current = q.pop();

            if (current->left == nullptr){
                current -> left = new NodeT{value, nullptr, nullptr};
                break;
            } else{
                q.push(current -> left);
            }

            if (current->right == nullptr){
                current -> right = new NodeT{value, nullptr, nullptr};
                break;
            } else{
                q.push(current -> right);
            }
        }

        q.clear();
    }

    bool checkNode(NodeT* node){
        if ((node -> right == nullptr && node -> left == nullptr)) return true;

        if ((node -> right == nullptr && node -> left != nullptr) || ((node -> right != nullptr && node -> left == nullptr))) return false;
        else{
            return (checkNode(node ->right) && checkNode(node -> left));
        }
    }

    bool checkFull(){
        return checkNode(root);
    }

    int max_length(){
        int m = 0;
        Queue<NodeT*> q;
        q.push(root);
        while(q.length != 0){
            NodeT* current = q.pop();
            string data = current->data;
            if (m < data.length()){
                m = data.length();
            }


            if (current->left != nullptr){
                q.push(current -> left);
            }

            if (current->right != nullptr){
                q.push(current -> right);
            }
        }

        q.clear();

        return m;
    }

    void get(List<T>& res){
        Queue<NodeT*> q;
        q.push(root);
        while(q.length != 0){
            NodeT* current = q.pop();
            string data = current->data;
            res.push(data);


            if (current->left != nullptr){
                q.push(current -> left);
            }

            if (current->right != nullptr){
                q.push(current -> right);
            }
        }

        q.clear();
    }

    void print(){
        printTreeHelper(root);
    }

    int getHeight(NodeT* node){
        int res = 0;
        while (node -> left != nullptr){
            node = node->left;
            res++;
        }
        return res;
    }

    void printTreeHelper(NodeT* node){
        int maxx = max_length();
         if (root == nullptr) {
            return;
        }

        Queue<NodeT*> q;
        q.push(root);
        int level = 0;
        int maxLevel = getHeight(root);

        while (q.length != 0) {
            int size = q.length;
            int padding = (maxLevel - level) * maxx + 3 * (maxLevel - level);

            for (int i = 0; i < size; ++i) {
                NodeT* node = q.pop();

                if (node != nullptr) {
                    std::cout << std::setw(padding) << " ";
                    for (int i = 0; i < level; i++){
                        cout << ' ';
                    }
                    std::cout << node->data;
                    std::cout << std::setw(padding) << " ";
                    for (int i = 0; i < level; i++){
                        cout << ' ';
                    }

                    q.push(node->left);
                    q.push(node->right);
                } else {
                    std::cout << std::setw(padding) << "";
                    std::cout << " ";
                    std::cout << std::setw(padding) << "";
                }
            }

            std::cout << std::endl;
            ++level;
        }

        q.clear();
    }

    void clear(){
        Queue<NodeT*> q;
        q.push(root);
        while(q.length != 0){
            NodeT* current = q.pop();


            if (!(current->left == nullptr)){
                q.push(current -> left);
            }

            if (!(current->right == nullptr)){
                q.push(current -> right);
            }
            delete current;
        }

        q.clear();
        root = nullptr;
    }



};