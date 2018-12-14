#include <string>
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include<map>
#include <sstream>
#include "MaxFibonacciHeap.h"
using namespace std;

/**
 * main function
*/
int main (int argc, char *argv[]){
	if(argc!=2){
		cout<<"Input file is not provided"<<endl;
		exit(1);
	}
    map<string,Node*> keyWordMap;                   //Hashmap where key is the string of words and value will be the pointers to the nodes.
    std::string::size_type st;
    string is;
    MaxFibonacciHeap h;
    std::ifstream input_file(argv[1]);              //Open input file.
    std::ofstream output_file("output_file.txt");   //Open output file.
    while (std::getline(input_file, is)){
        try{
            int z=std::stoi(is,&st);                //If its a query
            /*
            So it is an integer, Do remove max z times
            */
            Node **temparr=new Node*[z];
            int i;
			// perform z removeMax operations
			for(i=0;i<z;i++)
			{
				temparr[i]=h.removeMax();
				output_file<<(i>0?",":"")<<temparr[i]->keyWord;
			}
			output_file<<"\n";
			// reinsert removed nodes
			for(i=0;i<z;i++)
				h.insert(temparr[i]);
			delete[] temparr;		// dellocate array
        }catch(const std::invalid_argument& ia){
            /*
            split by space
            remove $ sign
            check if first string is in hashtable
            if yes, do increaseKey of the node it points to
            if no, do insert
            */
            if(is.compare("stop")==0){
                break;
            }
            vector<string> separateStrings;
            stringstream s(is);
            while(!s.eof()) {
                string tmp;
                s >> tmp;
                separateStrings.push_back(tmp);
            }
            string key=separateStrings.at(0).erase(0,1);
            int numOfOccurences=std::stoi(separateStrings.at(1),&st);
            if (keyWordMap.count(key)==0){
                Node *t=h.createNode(key,numOfOccurences);
                h.insert(t);
                keyWordMap[key]=t;
            }else{
                h.increaseKey(keyWordMap[key],numOfOccurences);
            }

        }
    }
	input_file.close();
	output_file.close();
	return 0;
}

