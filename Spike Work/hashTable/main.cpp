// Hash table for finding microsatellites
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

class HashTable{
public:
    // tuple value : count of number of microsat, start poition o full genome, end position of full genome, penalty score
    unordered_map<string, vector<int>> hashtable;

    void findSubMicrosat(const int minLen, const int maxLen, const string& genomeString){
        // Goes through each char in the string
        for(int startpos = 0; startpos < genomeString.length(); startpos ++) {
            // Goes through each len of microsat
            for (int i = minLen; i < maxLen + 1; i++) {
                string subMicrosat = genomeString.substr(startpos ,  i);
                if(subMicrosat.length() == i) { // chooses to ignore end values that may be not the right length - needs to fixed does extra computation
                    add(subMicrosat, startpos, startpos + i -1); // minus 1 to migigate 0 index
                }
            }
        }
    }

    void getRidOfLowNoMicrosat(){


    }

    unordered_map<string, vector<int>> checkIfNextToeachOther(){
        vector<int> trueOrFalse;
        unordered_map<string, vector<int>> mapOfFoundMicrosats;
        for (auto & [key, vec] : hashtable) { // for each vector in hastable

            if(vec[0] < 2){ // check if there is multiple sub micosats fo this sub microsats - cange 2 to min microsat len
                continue;
            }

            // checks if they are next to each other
            int startpos = vec[1]; // save the startPos of the potential microsat
            for (int i = 2; i + 4 < vec.size(); i += 3 ) { // starts at 2 to skip count and first start pos

                if (vec[i] + 1 == vec [i + 2] ){//checks if startpos
                    if ( i + 5  == vec.size()){ // checks if reached end of vector
                        //cout << "Here";
                        int endPos = vec[i + 3];
                        mapOfFoundMicrosats[key] = {startpos, endPos};
                        trueOrFalse.push_back(1);
                        break;
                    }
                }
                else{
                    trueOrFalse.push_back(0);
                    break; // break if one is not next to each other
                }
            }
        }
       return mapOfFoundMicrosats;
    }

    void calculatePenalty(){}

    void add (const string& microSat, int startpos, int endpos){
         int penaltyScore = 0; // NEED TO CHANGE TO ACTUALLY CALCULATE PENALTY.

        //check if the microsatllite is in the hashtable
        if (hashtable.find(microSat) == hashtable.end()) {
            // DON'T FORGET start pos is indexed from 1 but GFF file start indexing from 1
            vector<int> v = {1,startpos,endpos,penaltyScore};
            hashtable[microSat]= v; // add to the hash table with a count of 1
        }

        else{ //else increase the current count by 1
            vector initialVector = hashtable[microSat];

            // get old count and then add one to it
            int initialCount = initialVector[0];
            int newCount = initialCount + 1;

            // Make new vector
            replace(initialVector.begin(), initialVector.end(),initialVector[0], newCount);
            initialVector.insert(initialVector.end(),startpos);
            initialVector.insert(initialVector.end(), endpos);
            initialVector.insert(initialVector.end(), penaltyScore);

            // Assigning new values to key
            hashtable[microSat] = initialVector;

        }
    }

    void printHashTable(){
        // Printing of Unordered_MAP
        cout << "Microsat Count" << endl;
        for (auto & [key, vec] : hashtable) {
            cout << key << "    " <<endl;
            for (int i : vec) {
                cout << i << "  ";
            }
            cout << "\n" ;
        }
    }

};



int main() {
    HashTable hashtable;
    hashtable.findSubMicrosat(2,5,"GCTATATATATATATATATATATATATATATATATATATATATATATATATATATATATGGG");
    hashtable.printHashTable();
    for (auto& [key, vec] : hashtable.checkIfNextToeachOther()){
        cout << key << "    ";
        for (int i : vec){
            cout << "  " << i;
        }
         cout << "\n";
    }

}
