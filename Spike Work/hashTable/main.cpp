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
    int penaltyScore = 0;

    /**
     * Mismatch table
     */
    // used sting instead of array because of https://stackoverflow.com/questions/21946447/how-much-performance-difference-when-using-string-vs-char-array
    unordered_map<char, string> misMatchTbl = { // should i order values in most to least likeyly??
            {'A', "MRDHVWN"},
            {'C', "YSBHVMN"},
            {'G', "RKSBDVN"},
            {'T', "YKWBDHN"},
            {'M', "ACN"},
            {'R', "GAN"},
            {'Y', "TCN"},
            {'K', "GTN"},
            {'S', "GCN"},
            {'W', "ATN"},
            {'B', "GTCN"},
            {'D', "GATN"},
            {'H', "ACTN"},
            {'V', "GCAN"},
            {'N', "ACGTMRYKSWBDHV"},
    };

    // https://noobtuts.com/cpp/compare-float-values
    bool compareFloats(float currMismatchPerc, float mismatchPerc, float epsilon = 0.001f) {
        return (fabs(currMismatchPerc - mismatchPerc) < epsilon);
    }

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
    void
    findMicrosat(const string &sequence, int minLenRepeats, int maxLenRepeats, int minLenMicrosat, float mismatchPerc) {
        // Get length of the sequence
        int lenOfSequence = sequence.length();
        string noMismatchRepeat;
        string microSat;

        int noMismatches = 0;
        // UGHHHH wanted to do for loops the other way around - AMANDA
        for (int nucleotidePos = 0; nucleotidePos < lenOfSequence; nucleotidePos++) {
            for (int lenOfRepeats = minLenRepeats; lenOfRepeats <= maxLenRepeats; lenOfRepeats++) {
                if (nucleotidePos + lenOfRepeats >= lenOfSequence -1 ){break;}
                // Go through each nucleotide in sequence as microsat can start from any position

                // resetting values
                int startPos = nucleotidePos;
                float currMismatchPerc = 0.0;
                int endPos = -1; // set initial;y to -1 in case correct endpos hasn't been found
                int lenThreshold = 1; // check to see if repeats pass the len threshold fo it to be considered a microsatellite - HAS TO B ONE  cause else it will disclude the fist repeat

                // add first microsat
                microSat = sequence.substr(nucleotidePos, lenOfRepeats);
                while (compareRepeats(sequence.substr(nucleotidePos, lenOfRepeats),
                                      sequence.substr(nucleotidePos + lenOfRepeats, lenOfRepeats)) or
                       currMismatchPerc <= mismatchPerc) {

                    // updating mismatch percentage
                    if (not compareRepeats(sequence.substr(nucleotidePos, lenOfRepeats),
                                           sequence.substr(nucleotidePos + lenOfRepeats, lenOfRepeats))) {
                        noMismatches += 1;
                        currMismatchPerc = float(noMismatches) / float(microSat.length());

                    }
                    lenThreshold++;
                    microSat += sequence.substr(nucleotidePos + lenOfRepeats, lenOfRepeats);

                    // assigned to the end of the second substring, *2 as nucleotidePos is the start pos of the first substring
                    nucleotidePos += lenOfRepeats;
                    endPos = nucleotidePos + lenOfRepeats - 1;

                    // break out of loop if end of sequence
                    if (endPos >= lenOfSequence) {
                        break;
                    }

                }
                // if repeats meet the threshold output the result
                if (lenThreshold >= minLenMicrosat) {

                    cout << "Microsatellite found: " << microSat << endl;
                    string mostCommonRepeat = findMostCommonRepeatInMicoSat(microSat, lenOfRepeats);
                    //addToHashtable(sequence.substr(nucleotidePos, lenOfRepeats), startPos, endPos);
                    addToHashtable(mostCommonRepeat, startPos, endPos);
                }
            }
        }
        printHashTable();
    }

    /**
     *
     * @param microsat
     * @param repeatLen
     * @return most common repeat in the microsat
     */

    // this function is used after a micosat s found to find the most common repeat in the microsat so that the repeat value can be added to
    // the hashtable accurately instead of it being just the last repeat at the end of the micosat
    static string findMostCommonRepeatInMicoSat(const string &microsat,
                                                int repeatLen) { // can cause issues when finding repeats and adding to hashtable

        unordered_map<string, int> counter;
        string mostCommonRepeat;

        // counts the number of that specific repeat and adds them to the hashtable
        for (int i = 0; i + repeatLen <= microsat.length() - 1; i += repeatLen) {
            int initialCount = counter[microsat.substr(i, repeatLen)];
            int currentCount = initialCount + 1;
            counter[microsat.substr(i, repeatLen)] = currentCount;
        }

        // Printing of Unordered_MAP
        cout << "Counter HASHTABLE" << endl;
        for (auto &[key, vec]: counter) {
            cout << key << "    " << vec << endl;

        }
        int maxVal = -1;
        // If the counter contains any alpha numeric value that are not ATGC then it favours then next most common value GET RID
        for (auto &[key, val]: counter) {
            // Goes through the hashtable and returns the most common repeat
            if (val > maxVal) {
                // Goes through each char in the repeat to see if its not ATGC
                mostCommonRepeat = key;
                maxVal = val;
            }
        }

        return mostCommonRepeat;
    };

    /**
     *
     * @param firstRepeat
     * @param secondRepeat
     * @return bool if the repeats match
     */
    bool
    compareRepeats(string firstRepeat, string secondRepeat) { // both first and second repeat should be the same size
        int lenFirstRepeat = firstRepeat.length();
        // Goes through the bases in the first repeat
        for (int base = 0; base < lenFirstRepeat; base++) {
            // if the first base is equal to the second base i.e. A = A
            if (firstRepeat[base] == secondRepeat[base]) {
                continue;
            } else {
                string mismatches = misMatchTbl[firstRepeat[base]];
                for (int i = 0; i < mismatches.length(); i++) {
                    if (secondRepeat[base] == mismatches[i]) { break; }
                    else if (i == mismatches.length() - 1) {
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
    void addToHashtable(const string &repeat, int startPos, int endPos) {

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

    void findSequence() {

    }
};

int main() {
    // A repeat is the repeat in a micosat i.e. ATGATGATGATGATGATG the repeat = ATG
    string sequence;
    int minLenRepeats;
    int maxLenRepeats;
    int minLenMicrosat;
    float mismatchPerc;

    cout << "Enter the sequence: ";
    cin >> sequence;

    cout << "Enter the minimum length of the Microsat: ";
    cin >> minLenMicrosat;

    cout << "Enter the minimum length of repeats: ";
    cin >> minLenRepeats;

    cout << "Enter the maximum length of repeats: ";
    cin >> maxLenRepeats;

    cout << "Enter mismatches percentage allowed: ";
    cin >> mismatchPerc;

    MicrosatFinder microsatFinder;

    microsatFinder.findMicrosat(sequence, minLenRepeats, maxLenRepeats, minLenMicrosat, mismatchPerc);
}