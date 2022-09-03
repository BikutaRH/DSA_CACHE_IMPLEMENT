#ifndef CACHE_H
#define CACHE_H

#include "main.h"

class SearchEngine {
public:
    Elem** cache = new Elem*[MAXSIZE]; // cache prime
    Elem** hashTable = new Elem*[MAXSIZE]; // hash
    int size = 0; // size of cache
public:
    class Node; // forward declare
    Node* root;
public:
    // ham insert node trong bst
    Node* insert(Elem* element, int index, Node*& root){
        if(root == NULL){
            root = new Node(element,index, NULL, NULL);
            // cout << "node chen: " << root->element->addr << endl;
        }
        else if(element->addr < root->element->addr){
            root->left = insert(element, index, root->left);
        }
        else if(element->addr > root->element->addr){
            root->right = insert(element, index, root->right);
        }
        return root; 
    }
    // ham xoa node trong bst
    void delNode(Node*& root, Elem* element){
        if(root == NULL) return;
        if(element->addr < root->element->addr) delNode(root->left, element);
        else if(element->addr > root->element->addr)delNode(root->right, element);
        else{
            if(root->left == NULL){
                Node* temp = root;
                // cout << "node xoa: " << temp->element->addr << endl;
                root = root->right;
                delete temp;
            }
            else if(root->right == NULL){
                Node* temp = root;
                // cout << "node xoa: " << temp->element->addr << endl;
                root = root->left;
                delete temp;
            }
            else{
                Node* temp = root->right;
                while(temp->left != NULL) temp = temp->left;
                root->element = temp->element;
                delNode(root->right, temp->element);
            }
        }
    }
    // ham search addr trong bst
    void search(Node*& root, int& index, bool& exist, int addr){
        if(root == NULL) return;
        if(addr < root->element->addr) search(root->left, index, exist, addr);
        else if (addr > root->element->addr) search(root->right, index, exist, addr);
        else{
            index = root->index;
            exist = true;
            return;
        }
    }
    // ham xoa bst
    Node* makeEmpty(Node*& root){
        if(root == NULL) return NULL;
        else{
            makeEmpty(root->left);
            makeEmpty(root->right);
            delete root;
        }
        return NULL;
    }
    // ham in trung thu tu
    void printInOrder(Node*& root){
        if(root == NULL){
            return;
        }
        printInOrder(root->left);
        cout << root->element->addr << " " << root->element->data->getValue() << " " << (root->element->sync?"true":"false") << endl;
        printInOrder(root->right);
    }
    // ham in tien thu tu
    void printPreOrder(Node*& root){
        if(root == NULL){
            return;
        } 
        cout << root->element->addr << " " << root->element->data->getValue() << " " << (root->element->sync?"true":"false") << endl;
        printPreOrder(root->left);
        printPreOrder(root->right);
    }
    // ham in bst
    void PrintBST(){
        cout << "Print BST in inorder:\n";
        printInOrder(root);
        cout << "Print BST in preorder:\n";
        printPreOrder(root);
    }
    // hasing function
    int hashFunction(int key){
        return key % MAXSIZE;
    }
    void insertHash(Elem* element){
        int index = hashFunction(element->addr);
        // cout << index << " ";
        if (hashTable[index] == NULL){
            hashTable[index] = element;
            // cout << "value: " << hashTable[index]->addr << endl;
            return;
        } 
        while(hashTable[index] != NULL){
            index++;
            index = index % MAXSIZE;
        }
        if (hashTable[index] == NULL) hashTable[index] = element;
    }

    class Node{
    public:
        Elem* element;
        int index; // index cua element trong cache
        Node* left;
        Node* right;
    public:
        Node(){
            element = NULL;
            index = 0;
            left = NULL;
            right = NULL;
        }
        Node(Elem* element, int index, Node* left, Node* right){
            this->element = element;
            this->index = index;
            this->left = left;
            this->right = right;
        }
        ~Node(){};
};    
    virtual void setRoot(){};     
    SearchEngine(){
        for(int i = 0; i < MAXSIZE; i++){
            cout << "ham tao seachr" << endl;
            cache[i] = NULL;
        }
    }
    ~SearchEngine(){
        root = makeEmpty(root);
        for(int i = 0; i < size; i++){
            cout << "ham huy seachr" << endl;
            delete cache[i];
        } 
        delete[] cache;
        delete[] hashTable;
    }
};

class ReplacementPolicy {
public:
    class Node;
    Node** buffer = new Node*[MAXSIZE];
    int curSizeBuffer;

public:
    void virtual printRPlace(Elem**& arr){};
    void virtual swap(Node**&, int, int){};
    void virtual sortUp(Node**& list, int pos){};
    void virtual sortDown(Node**& list, int pos){}
    void virtual bubbleSort(Node**&, int pos){};
    void virtual reheapUp(Node**&, int pos){};
    void virtual reheapDown(Node**&, int pos){};
    void virtual update(Node**&, int pos){};
    void virtual insert(Node**&, int index, int counter){};
    void virtual remove(Node**&, int pos){};
    void virtual putBuffer(Node**&, int index, int counter){};
public:
    class Node {
    public:
        int indexOfElem;
        int counter;
    public:
        Node(int index, int counter){
            this->indexOfElem = index;
            this->counter = counter;
        }
        ~Node(){};
    };
};

class MFU : public ReplacementPolicy {
    void swap(Node**& maxHeap, int child, int parent){
        Node* temp = maxHeap[child];
        maxHeap[child] = maxHeap[parent];
        maxHeap[parent] = temp;
    }

    void reheapUp(Node**& maxHeap, int pos){   
        if (pos > 0) {
            int parent = (pos-1)/2;
            if (maxHeap[pos]->counter > maxHeap[parent]->counter) {
                swap(maxHeap, pos, parent);
                reheapUp(maxHeap, parent);
            }
        } else return;
    }

    void reheapDown(Node**& maxHeap, int pos){   
        int leftChild = pos*2 + 1;
        int rightChild = pos*2 + 2;
        int maxChild;
        if (leftChild <= curSizeBuffer - 1) {
            if (rightChild <= curSizeBuffer - 1 && maxHeap[rightChild]->counter >= maxHeap[leftChild]->counter)
                maxChild = rightChild;
            else maxChild = leftChild;
            if (maxHeap[maxChild]->counter >= maxHeap[pos]->counter) {
                swap(maxHeap, maxChild, pos);
                reheapDown(maxHeap, maxChild);
            }
        }
    }

    void remove(Node**& maxHeap, int pos){

        if(curSizeBuffer == 1){
            delete maxHeap[0];
            maxHeap[0] = NULL;
            curSizeBuffer--;
            return;
        }else if(pos == curSizeBuffer - 1){
            delete maxHeap[pos];
            maxHeap[pos] = NULL;
            curSizeBuffer--;
            return;
        }else{
            Node* temp = maxHeap[pos];
            maxHeap[pos] = maxHeap[curSizeBuffer - 1];
            delete temp;
            maxHeap[curSizeBuffer - 1] = NULL;
            curSizeBuffer--;
            if(maxHeap[pos]->counter > maxHeap[(pos-1)/2]->counter) reheapUp(maxHeap, pos);
            else reheapDown(maxHeap, pos);
        }
        
    }

    void insert(Node**& maxHeap, int index, int counter){
        Node* newNode =  new Node(index, counter);
        maxHeap[curSizeBuffer] = newNode;
        curSizeBuffer++;
        reheapUp(maxHeap, curSizeBuffer - 1);    
    }

    void update(Node**& maxHeap, int index){
        // cout << "update: ";
        int temp = 0;
        int posInHeap = 0;
        for(int i = 0; i < curSizeBuffer; i++){
            // cout << maxHeap[i]->indexOfElem << " " << maxHeap[i]->counter << endl;
            if (maxHeap[i]->indexOfElem == index){
                temp = maxHeap[i]->counter;
                posInHeap = i; 
                // cout << temp << " " << i << endl;
                break;
            }  
        }
        remove(maxHeap, posInHeap);
        insert(maxHeap, index, temp + 1);
    }

    void putBuffer(Node**& maxHeap, int index, int counter){
        remove(maxHeap, 0);
        insert(maxHeap, index, counter);
    }

    void printRPlace(Elem**& arr){
        for(int i = curSizeBuffer - 1 ; i >=0 ; i--){
            cout << arr[buffer[i]->indexOfElem]->addr << " " << arr[buffer[i]->indexOfElem]->data->getValue() << " " << (arr[buffer[i]->indexOfElem]->sync?"true":"false") << endl;
        }
    }
};

class LFU : public ReplacementPolicy {
    void swap(Node**& minHeap, int child, int parent){
        Node* temp = minHeap[child];
        minHeap[child] = minHeap[parent];
        minHeap[parent] = temp;
    }

    void reheapUp(Node**& minHeap, int pos){   
        if (pos > 0) {
            int parent = (pos-1)/2;
            if (minHeap[pos]->counter < minHeap[parent]->counter) {
                swap(minHeap, pos, parent);
                reheapUp(minHeap, parent);
            }
        } else return;
    }

    void reheapDown(Node**& minHeap, int pos){   
        int leftChild = pos*2 + 1;
        int rightChild = pos*2 + 2;
        int minChild;
        if (leftChild <= curSizeBuffer - 1) {
            if (rightChild <= curSizeBuffer - 1 && minHeap[rightChild]->counter < minHeap[leftChild]->counter)
                minChild = rightChild;
            else minChild = leftChild;
            if (minHeap[minChild]->counter <= minHeap[pos]->counter) {
                swap(minHeap, minChild, pos);
                reheapDown(minHeap, minChild);
            }
        }
    }

    void remove(Node**& minHeap, int pos){

        if(curSizeBuffer == 1){
            delete minHeap[0];
            minHeap[0] = NULL;
            curSizeBuffer--;
            return;
        }else if(pos == curSizeBuffer - 1){
            delete minHeap[pos];
            minHeap[pos] = NULL;
            curSizeBuffer--;
            return;
        }else{
            Node* temp = minHeap[pos];
            minHeap[pos] = minHeap[curSizeBuffer - 1];
            delete temp;
            minHeap[curSizeBuffer - 1] = NULL;
            curSizeBuffer--;
            if(minHeap[pos]->counter < minHeap[(pos-1)/2]->counter) reheapUp(minHeap, pos);
            else reheapDown(minHeap, pos);
        }
        
    }

    void insert(Node**& minHeap, int index, int counter){
        Node* newNode =  new Node(index, counter);
        minHeap[curSizeBuffer] = newNode;
        curSizeBuffer++;
        reheapUp(minHeap, curSizeBuffer - 1);    
    }

    void update(Node**& minHeap, int index){
        // cout << "update: ";
        int temp = 0;
        int posInHeap = 0;
        for(int i = 0; i < curSizeBuffer; i++){
            // cout << minHeap[i]->indexOfElem << " " << minHeap[i]->counter << endl;
            if (minHeap[i]->indexOfElem == index){
                temp = minHeap[i]->counter;
                posInHeap = i; 
                // cout << temp << " " << i << endl;
                break;
            }  
        }
        remove(minHeap, posInHeap);
        insert(minHeap, index, temp + 1);
    }

    void putBuffer(Node**& minHeap, int index, int counter){
        remove(minHeap, 0);
        insert(minHeap, index, counter);
    }

    void printRPlace(Elem**& arr){
        for(int i = 0 ; i < curSizeBuffer ; i++){
            cout << arr[buffer[i]->indexOfElem]->addr << " " << arr[buffer[i]->indexOfElem]->data->getValue() << " " << (arr[buffer[i]->indexOfElem]->sync?"true":"false") << endl;
        }
    } 
};

class MFRU: public ReplacementPolicy {

    void swap(Node**& list, int child, int parent){
        Node* temp = list[child];
        list[child] = list[parent];
        list[parent] = temp;
    }

    void sortUp(Node**& list, int pos){   
        if (pos > 0) {
            int parent = (pos-1);
            if (list[pos]->counter >= list[parent]->counter) {
                swap(list, pos, parent);
                sortUp(list, parent);
            }
        } else return;
    }

    void sortDown(Node**& list, int pos){   
        if (pos < curSizeBuffer - 1) {
            int child = (pos+1);
            if (list[pos]->counter < list[child]->counter) {
                swap(list, pos, child);
                sortDown(list, child);
            }
        } else return;
    }

    void insert(Node**& list, int index, int counter){
        Node* newNode =  new Node(index, counter);
        list[curSizeBuffer] = newNode;
        curSizeBuffer++;
        sortUp(list, curSizeBuffer - 1);    
    }

    void update(Node**& list, int index){
        for(int i = 0; i < curSizeBuffer; i++){
            if (list[i]->indexOfElem == index){
                list[i]->counter++;
                sortUp(list, i);
                break;
            }  
        }
    }

    void putBuffer(Node**& list, int index, int counter){
        list[0]->indexOfElem = index;
        list[0]->counter = 0;
        sortDown(list, 0);
    }

    void printRPlace(Elem**& arr){
        for(int i = 0 ; i < curSizeBuffer ; i++){
            cout << arr[buffer[i]->indexOfElem]->addr << " " << arr[buffer[i]->indexOfElem]->data->getValue() << " " << (arr[buffer[i]->indexOfElem]->sync?"true":"false") << endl;
        }
    }
};

class LFRU: public ReplacementPolicy {
    //TODO
    void swap(Node**& list, int child, int parent){
        Node* temp = list[child];
        list[child] = list[parent];
        list[parent] = temp;
    }

    void sortUp(Node**& list, int pos){   
        if (pos > 0) {
            int parent = (pos-1);
            if (list[pos]->counter < list[parent]->counter) {
                swap(list, pos, parent);
                sortUp(list, parent);
            }
        } else return;
    }

    void sortDown(Node**& list, int pos){   
        if (pos < curSizeBuffer - 1) {
            int child = (pos+1);
            if (list[pos]->counter >= list[child]->counter) {
                swap(list, pos, child);
                sortDown(list, child);
            }
        } else return;
    }

    void insert(Node**& list, int index, int counter){
        Node* newNode =  new Node(index, counter);
        list[curSizeBuffer] = newNode;
        curSizeBuffer++;
        sortUp(list, curSizeBuffer - 1);    
    }

    void update(Node**& list, int index){
        for(int i = 0; i < curSizeBuffer; i++){
            if (list[i]->indexOfElem == index){
                list[i]->counter++;
                sortDown(list, i);
                break;
            }  
        }
    }

    void putBuffer(Node**& list, int index, int counter){
        // cout << " truong hop 4 " << endl;
        list[0]->indexOfElem = index;
        list[0]->counter = 0;
        sortDown(list, 0);
    }

    void printRPlace(Elem**& arr){
        for(int i = 0 ; i < curSizeBuffer ; i++){
            cout << arr[buffer[i]->indexOfElem]->addr << " " << arr[buffer[i]->indexOfElem]->data->getValue() << " " << (arr[buffer[i]->indexOfElem]->sync?"true":"false") << endl;
        }
    }
};

class BST : public SearchEngine {
public:
    void setRoot(){
        root = NULL;
    }
};
#endif