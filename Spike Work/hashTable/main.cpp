// Hash table for finding microsatellites
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

class MicrosatFinder{
public:
    unordered_map<string, vector<int>> hashtable;
    /**
     *
     * @param sequence
     * The sequence that the user inputs for microsatllite analysis
     * @param minLenRepeats
     * This is the minimum length of the repeats in the microsatellite
     * @param maxLenRepeats
     * This is the maximum length of the repeats in the microsatellite
     * @param minLenMicrosat
     * This is the minimum length of repeats for the repeats to b considered a microsatellite
     * For example if the minimum length is 3 of repeat ATG then ATGATG will not but considered as a Microsatlite but ATGATGATG will be.
     */

    // need to deal with mismatches
    void findMicrosat(string sequence, int minLenRepeats, int maxLenRepeats, int minLenMicrosat){
        // Get length of the sequence
        int lenOfSequence = sequence.length();

        // Go through each nucleotide in sequence as microsat can start from any position
        for(int nucleotidePos = 0; nucleotidePos <= lenOfSequence - minLenRepeats; nucleotidePos ++){
            for(int lenOfRepeats = minLenRepeats; lenOfRepeats <= maxLenRepeats; lenOfRepeats ++){
                // check if end of sequence
                if(nucleotidePos + lenOfRepeats >= lenOfSequence){
                    break;
                }

                // check if the repeat repeats itself
                int startPos = nucleotidePos;
                int endPos = -1; // set initial;y to -1 in case correct endpos hasn't been found
                int lenThreshold = 0; // check to see if repeats pass the len threshold fo it to be considered a microsatellite
                while(sequence.substr(nucleotidePos, lenOfRepeats) == sequence.substr(nucleotidePos + lenOfRepeats, lenOfRepeats)){
                    // break at end of sequence
                    if(nucleotidePos + lenOfRepeats == lenOfSequence){ break;} // MAY NOT NEED ?
                    endPos = nucleotidePos + lenOfRepeats;
                    lenThreshold ++;
                    nucleotidePos += lenOfRepeats;
                }

                // if repeats meet the threshold output the result
                if(lenThreshold >= minLenMicrosat){
                    cout << sequence.substr(nucleotidePos, lenOfRepeats) << "   StartPos: " << startPos << "   EndPos: " << endPos << endl;
                    addToHashtable(sequence.substr(nucleotidePos, lenOfRepeats), startPos, endPos);
                }
            }
        }
        printHashTable();
    }

    // need to find which sequence it is in
    void addToHashtable(string repeat, int startPos, int endPos){

        // If repeat is not in hashtable - if micosat hasn't been found already
        if(hashtable.find(repeat) == hashtable.end()){
            hashtable[repeat] = {startPos, endPos};
        }

        // if microsat has already been found in another position
        else {
            vector<int> vec = hashtable[repeat];
            // Updating the vector with additional start and stop positions
            vec.push_back(startPos);
            vec.push_back(endPos);
            hashtable[repeat] = vec;
        }
    }

    void printHashTable() {
        // Printing of Unordered_MAP
        cout << "HASHTABLE" << endl;
        for (auto &[key, vec]: hashtable) {
            cout << "Key: " << key << "    " << endl;
            cout << "Start and end positions: ";
            for (int i: vec) {
                cout << i << "  ";
            }
            cout << "\n";
        }
    }
};

int main() {
    // A repeat is the repeat in a micosat i.e. ATGATGATGATGATGATG the repeat = ATG
    string sequence;
    int minLenRepeats;
    int maxLenRepeats;
    int minLenMicrosat;

    cout << "Enter the sequence: ";
    cin >> sequence;

    cout << "Enter the minimum length of the Microsat: ";
    cin >> minLenMicrosat;

    cout << "Enter the minimum length of repeats: ";
    cin >> minLenRepeats;

    cout << "Enter the maximum length of repeats: ";
    cin >> maxLenRepeats;



    MicrosatFinder microsatFinder;
    microsatFinder.findMicrosat(sequence, minLenRepeats, maxLenRepeats, minLenMicrosat);
}