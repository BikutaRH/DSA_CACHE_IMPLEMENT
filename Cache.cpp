#include "main.h"

Cache::Cache(SearchEngine* s,ReplacementPolicy* r):rp(r),s_engine(s) {}
Cache::~Cache(){
	delete rp;
	delete s_engine;
}

Data* Cache::read(int addr) {
    if(s_engine->size == 0) return NULL;
    else{
        bool exist = false;
        int index;
        s_engine->search(s_engine->root, index, exist, addr);
        if(exist){
            rp->update(rp->buffer, index);
            // rp->printBuffer();
            return s_engine->cache[index]->data;
        }else return NULL;
    }
}

Elem* Cache::put(int addr, Data* cont) {
    Elem* newElem = new Elem(addr, cont, true);
    if(s_engine->size < MAXSIZE){
        s_engine->cache[s_engine->size] = newElem; 
        s_engine->insert(newElem, s_engine->size, s_engine->root);
        rp->insert(rp->buffer, s_engine->size, 0);
        // rp->printBuffer();
        s_engine->size++;
        return NULL;
    }else{
        int indexToPut = rp->buffer[0]->indexOfElem;
        Elem* elemReturn = s_engine->cache[indexToPut];
        s_engine->cache[indexToPut] = newElem;
        s_engine->delNode(s_engine->root, elemReturn);
        s_engine->insert(newElem, indexToPut, s_engine->root);
        // rp->printBuffer();
        rp->putBuffer(rp->buffer, indexToPut, 0);
        return elemReturn;
    }
}

Elem* Cache::write(int addr, Data* cont) {
    bool exist = false;
    int index;
    s_engine->search(s_engine->root, index, exist, addr);
    if(exist){
        s_engine->cache[index]->data = cont;
        s_engine->cache[index]->sync = false;
        rp->update(rp->buffer, index);
        // rp->printBuffer();
        // cout << "testWrite " << index << " " << s_engine->cache[index]->data->getValue() << endl;
        // cout << addr << " " << index << endl;
        return NULL;
    }else{
        Elem* newElem = new Elem(addr, cont, false);
        if(s_engine->size < MAXSIZE){
            s_engine->cache[s_engine->size] = newElem; 
            s_engine->insert(newElem, s_engine->size, s_engine->root);
            rp->insert(rp->buffer, s_engine->size, 0);
            // rp->printBuffer();
            s_engine->size++;
            return NULL;
        }else{
            int indexToPut = rp->buffer[0]->indexOfElem;
            Elem* elemReturn = s_engine->cache[indexToPut];
            s_engine->cache[indexToPut] = newElem;
            s_engine->delNode(s_engine->root, elemReturn);
            s_engine->insert(newElem, indexToPut, s_engine->root);
            rp->putBuffer(rp->buffer, indexToPut, 0);
            // rp->printBuffer();
            return elemReturn;
        }
    }
}
void Cache::printRP() {
    rp->printRPlace(s_engine->cache);
}
void Cache::printSE() {
    s_engine->PrintBST();
}
void Cache::printLP() {
    cout << "Prime memory\n";
    for(int i = 0; i < s_engine->size; i++){
        cout << s_engine->cache[i]->addr << " " << s_engine->cache[i]->data->getValue() << " " << (s_engine->cache[i]->sync?"true":"false") << endl;
        // s_engine->hashTable[i] = NULL;
    }
    // cout << "Hash table memory\n";
    // for(int i = 0; i < s_engine->size; i++){
    //     s_engine->insertHash(s_engine->cache[i]);
    // }
    // for(int j = 0; j < s_engine->size; j++){
    //     cout << s_engine->hashTable[j]->addr << " " << s_engine->hashTable[j]->data->getValue() << " " << (s_engine->hashTable[j]->sync?"true":"false") << endl;
    // }
}
