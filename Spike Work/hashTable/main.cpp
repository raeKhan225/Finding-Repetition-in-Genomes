// Hash table for finding microsatellites
#include <iostream>
#include <unordered_map>
using namespace std;

class HashTable{
public:
    unordered_map<string, int> hashtable;


    void add (const string& microSat){
        //check if the microsatllite is in the hashtable
        if (hashtable.find(microSat) == hashtable.end()) { //if its not in the hashtable
            hashtable[microSat] = 1; // add to the hash table with a count of 1
        }
        else{ //else increase the current count by 1
            int initialCount = hashtable[microSat];
            hashtable[microSat] = initialCount + 1;
        }

    }

    void printHashTable(){
        // Printing of Unordered_MAP
        cout << "Microsat Count" << endl;
        for (auto i : hashtable)
            cout << i.first << "    " << i.second << endl;
    }

};

int main() {
    HashTable hashtable;
    hashtable.add("AAA");
    hashtable.add("AAA");
    hashtable.add("AAA");
    hashtable.add("AAT");

    hashtable.printHashTable();
}
