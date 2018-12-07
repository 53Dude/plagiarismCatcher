//
//  main.cpp
//  plagiarismCatcher
//
//  Created by Guy on 12/5/18.
//  Copyright © 2018 Guy Co. All rights reserved.
//

////////////////////    INCLUDES    ////////////////////
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

////////////////////    CONSTANTS    ////////////////////
#define MAX_FILE_NUM 1355
#define HASH_SIZE 1000000

using namespace std;

////////////////////    DECLARES NODE STRUCTURE FOR THE HASH TABLE    ////////////////////
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

////////////////////    KEY -> INDEX IN HASH TABLE    ////////////////////
int hashI(string key){
    
    int ans=0;
    for (int i = 0; i < key.size(); i++) {
        ans += 31*ans + key.at(i); //// sums characters ascii values with the past values*31
        ans%=HASH_SIZE; //// modulus by hash table size to keep the index in range
    }
    
    return ans;
}

////////////////////    FILE IDX -> HASH TABLE W/ CHECKING 4 COLLISIONS    ////////////////////
void put(string key,int curFile){
    
    int idx=hashI(key); //// gets hash table index for key
    hashNode *temp=table[idx];
    
    if(temp==NULL){ //// if no data at index the node is added
        temp = new hashNode(curFile);
        
        temp->next=table[idx];
        table[idx]=temp;
        return;
    }
    
    while(temp->next!=NULL){ //// if there are already nodes the new node is only added if the current file isn't already indexed
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

////////////////////    GIVEN DIRECTORY PULL PROGRAM    ////////////////////
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

////////////////////    FORMATS N LENGTH STRINGS FOR HASHING    ////////////////////
string strFormat(string input){
    string rtn;
    for(int i = 0; i < input.length(); i++) {
        if(((int)input[i]>64&&(int)input[i]<91)||((int)input[i]>96&&(int)input[i]<123)) //// only adds alphabetical characters to the string
            rtn += input[i];
    }
    transform(rtn.begin(), rtn.end(), rtn.begin(), ::tolower); //// makes entire string lowercase

    return rtn;
}

////////////////////    NODE DECL FOR ALREADY PRINTED PAIRS    ////////////////////
class dnEntry{
    // this is here because basically my conversion from the hash table to the 2D array
    // didn't work (sorry :( ) so I made this to hold all the pairs completed elements
    // to check with so I don't get a double print out. I understand that it makes my
    // program a little bit slower and it's not the most "elegant" solution but tbh I'm
    // tired and the most elegant solutions are for programs I'm being paid for, pls
    // don't be cross with me. Also, while I'm being honest I used two global variables
    // in this program even though I know I'm not supposed to and I would be glad to
    // explain and defend my use of them, you can email me at gsexton@utexas.edu or
    // call me at 8062528544 and we can "hash" it out (; Thank you for reading and I
    // hope this is also ample commenting to not get points taken off like i have for
    // literally every other program. Thanks and have a great break!
public:
    int x;
    int y;
};
vector<dnEntry> dne;

////////////////////    CHECKS X&Y TO SEE IF THEY HAVE BEEN DONE BEFORE, SEE ABOVE PARAGRAPH FOR LONG EXPLANATION    ////////////////////
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

////////////////////    THE "MAIN" EVENT    ////////////////////
int main(int argc, const char * argv[]) {
    // insert code here...
    
/////////////////////////// REPLACES ARGV[1&2] FOR XCODE TESTING
//argv[1]="sm_doc_set";
//argv[2]="6";
//////////////////////////
    
    const string& cPath = argv[1];
    const int n = atoi(argv[2]);
    
//cout << "Path: " << cPath << endl;
//cout << "n: " << n << endl << endl;
    
    vector<string> files = vector<string>();
    getdir(cPath,files);
    files.erase(files.begin(),files.begin()+2); //// deletes "." & ".."
    
    string wPath=cPath+"/";
    vector<string> cFile = vector<string>();
    string tempWord;
    
    for (unsigned int i = 0;i < files.size();i++) { //// does this for every file
//cout << endl << i << ": " << wPath+files[i] << endl;
        
        fstream wFile;
        wFile.open((wPath+files[i]).c_str());
        while(wFile >> tempWord&&cFile.size()<n){
                tempWord=strFormat(tempWord); //// reads in words and removes punctuations and spaces then adds to working vector of words
                cFile.push_back(tempWord);
//cout<< tempWord;
        }
        tempWord="";
        for(int i=0;i<n;i++){
            tempWord+=cFile.at(i); //// concatonates first 6 words from vector into one string and removes the first word from the vector
        }
        cFile.erase(cFile.begin());
        put(tempWord,i); //// adds string to hash table
        
        while(wFile >> tempWord){ //// continues this process until eof
            tempWord=strFormat(tempWord);
            cFile.push_back(tempWord);
            tempWord="";
            for(int i=0;i<n;i++){
                tempWord+=cFile.at(i);
            }
            cFile.erase(cFile.begin());
            put(tempWord,i);
        }
        cFile.clear(); //// clears out vector for next file to use
    }
    
    int sim[MAX_FILE_NUM ][MAX_FILE_NUM ]; //// 2D array of similarities between files (collisions in the hash table), is initialized to 0 right after
    for(int i=0;i<files.size();i++)
        for(int j=0;j<files.size();j++)
            sim[i][j]=0;
    
    hashNode *temp;
    hashNode *temp2;
    hashNode *del;
    
    for(int i=0;i<HASH_SIZE;i++){ //// scans the hash table and counts the amount of collosions in the table and adds to the corresponding cell in the sim array
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
        while(table[i]!=NULL){ //// deallocates the hash table
            del=table[i];
            table[i]=table[i]->next;
            delete(del);
        }
    }
    
    int bigX=0;
    int bigY=0;
    bool cont=true;
    
    while(cont){ //// searches through for the biggest element and prints it out then empties the cell in sim and does it again until no remaining cells are bigger than 200
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
    
    //// all done! Thanks for reading through my code! Have a great break and thank y'all again for a great semester!!
    
//std::cout << "Hello, World!\n";
    return 0;
}
