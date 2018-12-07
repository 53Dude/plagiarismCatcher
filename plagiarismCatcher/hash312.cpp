//
//  hash312.cpp
//  plagiarismCatcher
//
//  Created by Guy on 12/5/18.
//  Copyright © 2018 Guy Co. All rights reserved.
//

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>

#define hashSize 1000000

using namespace std;

class hashNode {
 public:
    int fileI;
    hashNode* next;
    
    hashNode(){
        fileI=-1;
        next=NULL;
    }
    hashNode(int Fi){
        fileI=Fi;
        next=NULL;
    }
    
};

class hash312 {
    
    hashNode* table[hashSize];
    void insert(int hashI,int fileI);
    int hash2idx(int hashRaw);
    
public:
    
    int hash(string key);
    void put(string key,int fileI);
    
    
};

int hash312::hash(string key){
    
    int ans=7;
    for (int i = 0; i < key.size(); i++) {
        ans = ans*31 + key.at(i);
    }
    return ans;
    
}

int hash312::hash2idx(int hashRaw){
    
    return hashRaw%hashSize;
    
}

void hash312::put(string key,int fileI){
    
    int kHash=hash(key);
    int kIdx=hash2idx(kHash);
    
    hashNode *temp = new hashNode(fileI);
    
    temp->next=table[kIdx];
    table[kIdx]=temp;
    
}
