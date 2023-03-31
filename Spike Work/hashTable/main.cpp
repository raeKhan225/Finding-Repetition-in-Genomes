// Hash table for finding microsatellites
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

class MicrosatFinder {
public:
    /**
     * The hashtable key is the pat that is in the microsat
     * the vector stores the start and stop positions of where the micosat with that repeat occurs
     */
    unordered_map<string, vector<int>> hashtable;

    /**
     * Mismatch table
     */
    // used sting instead of array because of https://stackoverflow.com/questions/21946447/how-much-performance-difference-when-using-string-vs-char-array
     unordered_map<char, string> misMatchTbl = { // should i order values in most to least likeyly??
            {'A',"MRDHVWN"},
            {'C',"YSBHVMN"},
            {'G',"RKSBDVN"},
            {'T',"YKWBDHN"},
            {'M',"ACN"},
            {'R',"GAN"},
            {'Y',"TCN"},
            {'K',"GTN"},
            {'S',"GCN"},
            {'W',"ATN"},
            {'B',"GTCN"},
            {'D',"GATN"},
            {'H',"ACTN"},
            {'V',"GCAN"},
            {'N',"ACGTMRYKSWBDHV"},
     };

    /**
     * FindMicrosat find the micosatellites in a DNA sequence
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
    void findMicrosat(const string& sequence, int minLenRepeats, int maxLenRepeats, int minLenMicrosat) {
        // Get length of the sequence
        int lenOfSequence = sequence.length();
        string noMismatchRepeat = "";
        int penaltyScore = 0;

        // Go through each nucleotide in sequence as microsat can start from any position
        for (int nucleotidePos = 0; nucleotidePos <= lenOfSequence - minLenRepeats; nucleotidePos++) {
            //
            for (int lenOfRepeats = minLenRepeats; lenOfRepeats <= maxLenRepeats; lenOfRepeats++) {
                // check if end of sequence
                if (nucleotidePos + lenOfRepeats >= lenOfSequence) {
                    break;
                }

                // check if the repeat repeats itself
                int startPos = nucleotidePos;
                int endPos = -1; // set initial;y to -1 in case correct endpos hasn't been found
                int lenThreshold = 0; // check to see if repeats pass the len threshold fo it to be considered a microsatellite
                while (compareRepeats(sequence.substr(nucleotidePos, lenOfRepeats),sequence.substr(nucleotidePos + lenOfRepeats, lenOfRepeats))){
                    // assigned to the end of the second substring, *2 as nucleotidePos is the start pos of the first substring
                    lenThreshold++;
                    nucleotidePos += lenOfRepeats;
                    endPos = nucleotidePos + lenOfRepeats-1;

                    // need to plus 1 if the last two sub stings of sequence, and they match
                    if(endPos + 1 == lenOfSequence ){
                        lenThreshold ++;
                        break;
                    }
                }

                // if repeats meet the threshold output the result
                if (lenThreshold >= minLenMicrosat) {
                    cout << sequence.substr(nucleotidePos, lenOfRepeats) << "   StartPos: " << startPos << "   EndPos: "
                         << endPos << endl;
                    addToHashtable(sequence.substr(nucleotidePos, lenOfRepeats), startPos, endPos);
                }
            }
        }
        printHashTable();
    }

    bool compareRepeats(string firstRepeat, string secondRepeat){ // both first and second repeat should be the same size
        int lenFirstRepeat = firstRepeat.length();
        // Goes through the bases in the first repeat
        for(int base = 0; base < lenFirstRepeat; base ++){
            // if the first base is equal to the second base i.e. A = A
            if(firstRepeat[base] == secondRepeat[base]){
                continue;
            }
            else{
                string mismatches = misMatchTbl[firstRepeat[base]];
                for(int i=0; i < mismatches.length(); i++){
                    if(secondRepeat[base] == mismatches[i]){break;}
                    else if (i == mismatches.length() -1){
                        return false;
                    }
                }
            }
        }
        return true;
    }

    /**
     * Adds micosats to hashtable using the repeat as a key,
     * @param repeat
     * @param startPos
     * @param endPos
     */
    // need to find which sequence it is in
    void addToHashtable(const string& repeat, int startPos, int endPos) {

        // If repeat is not in hashtable - if micosat hasn't been found already
        if (hashtable.find(repeat) == hashtable.end()) {
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

    /**
     * Print out the hashtable for visual representation
     */

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

    cout << "Enter how many mismatches are allowed: ";
    cin >> maxLenRepeats;

    MicrosatFinder microsatFinder;
    microsatFinder.findMicrosat(sequence, minLenRepeats, maxLenRepeats, minLenMicrosat);
}