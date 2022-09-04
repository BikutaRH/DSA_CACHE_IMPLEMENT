#include "main.h"

Cache::Cache(SearchEngine* s,ReplacementPolicy* r):rp(r),s_engine(s) {}
Cache::~Cache(){
	delete rp;
	delete s_engine;
}

Data* Cache::read(int addr) {
    if(rp->size == 0) return NULL;
    else{
        bool exist = false;
        int index;
        s_engine->search(s_engine->root, index, exist, addr);
        if(exist){
            rp->update(rp->buffer, index);
            return rp->cache[index]->data;
        }else return NULL;
    }
}

Elem* Cache::put(int addr, Data* cont) {
    Elem* newElem = new Elem(addr, cont, true);
    if(rp->size < MAXSIZE){
        rp->cache[rp->size] = newElem; 
        s_engine->insert(newElem, rp->size, s_engine->root);
        rp->insert(rp->buffer, rp->size, 0);
        rp->size++;
        return NULL;
    }else{
        int indexToPut = rp->buffer[0]->indexOfElem;
        Elem* elemReturn = rp->cache[indexToPut];
        rp->cache[indexToPut] = newElem;
        s_engine->delNode(s_engine->root, elemReturn);
        s_engine->insert(newElem, indexToPut, s_engine->root);
        rp->putBuffer(rp->buffer, indexToPut, 0);
        return elemReturn;
    }
}

Elem* Cache::write(int addr, Data* cont) {
    bool exist = false;
    int index;
    s_engine->search(s_engine->root, index, exist, addr);
    if(exist){
        rp->cache[index]->data = cont;
        rp->cache[index]->sync = false;
        rp->update(rp->buffer, index);
        return NULL;
    }else{
        Elem* newElem = new Elem(addr, cont, false);
        if(rp->size < MAXSIZE){
            rp->cache[rp->size] = newElem; 
            s_engine->insert(newElem, rp->size, s_engine->root);
            rp->insert(rp->buffer, rp->size, 0);
            rp->size++;
            return NULL;
        }else{
            int indexToPut = rp->buffer[0]->indexOfElem;
            Elem* elemReturn = rp->cache[indexToPut];
            rp->cache[indexToPut] = newElem;
            s_engine->delNode(s_engine->root, elemReturn);
            s_engine->insert(newElem, indexToPut, s_engine->root);
            rp->putBuffer(rp->buffer, indexToPut, 0);
            return elemReturn;
        }
    }
}
void Cache::printRP() {
    rp->printRPlace(rp->cache);
}
void Cache::printSE() {
    s_engine->PrintBST();
}
void Cache::printLP() {
    cout << "Prime memory\n";
    for(int i = 0; i < rp->size; i++){
        if(rp->cache[i] != NULL) cout << rp->cache[i]->addr << " " << rp->cache[i]->data->getValue() << " " << (rp->cache[i]->sync?"true":"false") << endl;
    }
    cout << "Hash table memory\n";
    Elem** hashTable = new Elem*[MAXSIZE];
    // khoi tao gia tri NUll cho hash table
    for(int i = 0; i < MAXSIZE; i++){
        hashTable[i] = NULL;
    }
    // insert gia tri vao hastable
    for(int i = 0; i < rp->size; i++){
        if(rp->cache[i] != NULL) s_engine->insertHash(rp->cache[i], hashTable);
    }
    // in gia tri trong hash theo thu tu tang dan chi muc
    for(int j = 0; j < MAXSIZE; j++){
        if(hashTable[j] != NULL) cout << hashTable[j]->addr << " " << hashTable[j]->data->getValue() << " " << (hashTable[j]->sync?"true":"false") << endl;
    }
    delete[] hashTable;
}
