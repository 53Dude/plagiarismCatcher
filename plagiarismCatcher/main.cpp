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
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <stdlib.h>

#define MAX_FILE_NUM 1355
#define HASH_SIZE 1000000

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

hashNode* table[HASH_SIZE];

int hashI(string key){
    
    int ans=0;
    for (int i = 0; i < key.size(); i++) {
        ans += 31*ans + key.at(i);
        ans%=HASH_SIZE;
    }
    
    return ans;
}

void put(string key,int curFile){
    
    int idx=hashI(key);
    hashNode *temp=table[idx];
    
    if(temp==NULL){
        temp = new hashNode(curFile);
        
        temp->next=table[idx];
        table[idx]=temp;
        return;
    }
    
    while(temp->next!=NULL){
        if(temp->fileI==curFile){
            return;
        }
        else
            temp=temp->next;
    }
    temp = new hashNode(curFile);
    
    temp->next=table[idx];
    table[idx]=temp;
    
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
        if(((int)input[i]>64&&(int)input[i]<91)||((int)input[i]>96&&(int)input[i]<123))
            rtn += input[i];
    }
    transform(rtn.begin(), rtn.end(), rtn.begin(), ::tolower);

    return rtn;
}

class dnEntry{
public:
    int x;
    int y;
};
vector<dnEntry> dne;

bool done(int x,int y){
    
    for(int i=0;i<dne.size();i++){
        if((x==dne[i].x&&y==dne[i].y)||(x==dne[i].y&&y==dne[i].x))
            return true;
    }
    dnEntry temp;
    temp.x=x;
    temp.y=y;
    dne.push_back(temp);
    return false;
}

int main(int argc, const char * argv[]) {
    // insert code here...
    
/////////////////////////// REPLACES ARGV[1&2] FOR XCODE TESTING
argv[1]="big_doc_set";
argv[2]="6";
//////////////////////////
    
    const string& cPath = argv[1];
    const int n = atoi(argv[2]);
    
    cout << "Path: " << cPath << endl;
    cout << "n: " << n << endl << endl;
    
    vector<string> files = vector<string>();
    getdir(cPath,files);
    files.erase(files.begin(),files.begin()+2);
    
    string wPath=cPath+"/";
    vector<string> cFile = vector<string>();
    string tempWord;
    string openFrom;
    
    for (unsigned int i = 0;i < files.size();i++) {
    //cout << endl << i << ": " << wPath+files[i] << endl;
        
        fstream wFile;
        wFile.open((wPath+files[i]).c_str());
        while(wFile >> tempWord&&cFile.size()<6){
                tempWord=strFormat(tempWord);
                cFile.push_back(tempWord);
    //cout<< tempWord;
        }
        
        tempWord=cFile.at(0)+cFile.at(1)+cFile.at(2)+cFile.at(3)+cFile.at(4)+cFile.at(5);
        cFile.erase(cFile.begin());
        put(tempWord,i);
        
        while(wFile >> tempWord){
            tempWord=strFormat(tempWord);
            cFile.push_back(tempWord);
            tempWord=cFile.at(0)+cFile.at(1)+cFile.at(2)+cFile.at(3)+cFile.at(4)+cFile.at(5);
            cFile.erase(cFile.begin());
            put(tempWord,i);
        }
        cFile.clear();
    }
    
    int sim[MAX_FILE_NUM ][MAX_FILE_NUM ];
    for(int i=0;i<files.size();i++)
        for(int j=0;j<files.size();j++)
            sim[i][j]=0;
    
    hashNode *temp;
    hashNode *temp2;
    
    for(int i=0;i<HASH_SIZE;i++){
        if(table[i]!=NULL&&table[i]->next!=NULL){
            temp=table[i];
            temp2=table[i];
            while(temp!=NULL){
                while(temp2!=NULL){
                    sim[temp->fileI][temp2->fileI]++;
                    temp2=temp2->next;
                }
                temp=temp->next;
                temp2=table[i];
            }
        }
    }
    
    int bigX=0;
    int bigY=0;
    bool cont=true;
    
    while(cont){
        cont=false;
        for(int i=0;i<files.size();i++){
            for(int j=0;j<files.size();j++){
                if(sim[i][j]>sim[bigX][bigY]&&sim[i][j]>200){
                    bigX=i;
                    bigY=j;
                    cont=true;
                }
            }
        }
        if(bigX!=bigY&&!done(bigX,bigY)){
            cout << sim[bigX][bigY] << ": " << files[bigX] << "," << files[bigY] << endl;
        }
        sim[bigX][bigY]=0;
    }
            
    
    std::cout << "Hello, World!\n";
    return 0;
}
