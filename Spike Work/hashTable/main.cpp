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


    unordered_map<string, vector<int>> checkIfNextToeachOther( int minLen){
        vector<int> trueOrFalse;
        unordered_map<string, vector<int>> mapOfFoundMicrosats;
        for (auto & [key, vec] : hashtable) { // for each vector in hastable
            if(vec[0] <minLen){ // check if there is multiple sub micosats fo this sub microsats - cange 2 to min microsat len
                continue;
            }

            // checks if they are next to each other

             // save the startPos of the potential microsat
            for (int i = 2; i + 4 < vec.size(); i ++ ) {// starts at 2 to skip count and first start pos
                int startpos = vec[i];
                int endPos = -1;
                while (vec[i] + 1 == vec [i + 2] ){//checks if startpos // need to make it so it doesn't only stop at the end of submicrosat
                    endPos = vec[i + 3];
                    i += 3;
                }
                vector<int> oldVal = mapOfFoundMicrosats[key];
                oldVal.push_back(startpos);
                oldVal.push_back(endPos);
                if ( i + 5  == vec.size()){ // checks if reached end of vector
                    break;
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
    void test(){
        string dna = "ATGAGATCCACAGGAGTTGAGCTGACTGAGCTGAGCTGAGCTGAGCTGAGCAGCTACGAGCTGAGCTGAGCTGAGCTGAGCTGAGCTGAGC";
        int minRepeatLength = 2;
        int maxRepeatLength = 6;

        vector<string> microsatellites;
        vector<pair<int, int>> positions;
        for(int i = 0; i < dna.length(); i++) {
            for(int j = i+minRepeatLength; j <= i+maxRepeatLength && j <= dna.length(); j++) {
                string substring = dna.substr(i, j-i);
                int repeatCount = (j-i) / substring.length();
                string repeatedSubstring = string(repeatCount, substring[0]);
                if(substring == repeatedSubstring) {
                    microsatellites.push_back(substring);
                    positions.emplace_back(i, j-1);
                }
            }
        }
        cout << microsatellites.size() << endl;
        for(int i = 0; i < microsatellites.size(); i++) {
            cout << microsatellites[i] << " (start: " << positions[i].first << ", end: " << positions[i].second << ")" << endl;
        }
    }
};

int main() {
    HashTable hashtable;
    /*
    hashtable.findSubMicrosat(2,5,"ATATATGCATATAT");
    //hashtable.printHashTable();
    for (auto& [key, vec] : hashtable.checkIfNextToeachOther(3)){
        cout << key << "    ";
        for (int i : vec){
            cout << "  " << i;
        }
         cout << "\n";
    }*/
    hashtable.test();

}
