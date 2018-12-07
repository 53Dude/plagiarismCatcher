//
//  main.cpp
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

int hashK(string key){
    
    int ans=7;
    for (int i = 0; i < key.size(); i++) {
        ans = ans*31 + key.at(i);
    }
    return ans;
    
}

int hash2idx(int hashRaw){
    
    return hashRaw%hashSize;
    
}

void put(string key,int fileI,hashNode *tbl[]){
    
    int kHash=hashK(key);
    int kIdx=hash2idx(kHash);
    
    hashNode *temp = new hashNode(fileI);
    
    temp->next=tbl[kIdx];
    tbl[kIdx]=temp;
    
}

int getdir (string dir, vector<string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }
    
    while ((dirp = readdir(dp)) != NULL) {
        files.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}

string strFormat(string input){
        string rtn;
        for(int i = 0; i < input.length(); i++) {
            if(isalpha(input[i]))
                rtn += input[i];
        }
        return rtn;
}

int main(int argc, const char * argv[]) {
    // insert code here...
    
/////////////////////////// REPLACES ARGV[1&2] FOR XCODE TESTING
argv[1]="sm_doc_set";
argv[2]="6";
//////////////////////////
    
    const string& cPath = argv[1];
    const int n = atoi(argv[2]);
    
    cout << "Path: " << cPath << endl;
    cout << "n: " << n << endl << endl;
    
    vector<string> files = vector<string>();
    getdir(cPath,files);
    files.erase(files.begin(),files.begin()+2);
    
    hashNode* table[hashSize];
    string wPath=cPath+"/";
    vector<string> cFile = vector<string>();
    string tempWord;
    
for (unsigned int i = 0;i < files.size();i++) {
    cout << endl << i << ": " << wPath+files[i] << endl;
    
    ifstream wFile;
    wFile.open(wPath+files[i]);
    while(wFile >> tempWord){
        tempWord=strFormat(tempWord);
        transform(tempWord.begin(), tempWord.end(), tempWord.begin(), ::tolower);
        cFile.push_back(tempWord);
//cout<< tempWord;
    }
    
    while(cFile.size()>=n){
        tempWord=cFile.at(0)+cFile.at(1)+cFile.at(2)+cFile.at(3)+cFile.at(4)+cFile.at(5);
        cout << tempWord << endl;
        cFile.erase(cFile.begin());
    }
    
    cFile.clear();
}
    
    
    std::cout << "Hello, World!\n";
    return 0;
}
