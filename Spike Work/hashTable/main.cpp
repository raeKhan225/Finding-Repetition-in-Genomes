// Hash table for finding microsatellites
#include <iostream>
#include <unordered_map>
#include <vector>

class MicrosatFinder {
public:
    /**
     * The hashtable key is the pat that is in the microsat
     * the vector stores the start and stop positions of where the micosat with that repeat occurs
     */
    int penaltyScore = 0;
    int nomatchPenalty;
    int mismatchPenalty;
    std::unordered_map<std::string, std::vector<int>> hashtable;

    // Constructor // CHECK
    MicrosatFinder(int nomatchPenalty, int mismatchPenalty) {
        this -> nomatchPenalty = nomatchPenalty; // ie. if the expected val at that pos is C but is actually G
        this -> mismatchPenalty = mismatchPenalty; //ie. if the expected val at that pos is G but is actually a val in the mismatch table ie. N

    }

    /**
     * Mismatch table
     */
    // used sting instead of array because of https://stackoverflow.com/questions/21946447/how-much-performance-difference-when-using-string-vs-char-array
    std::unordered_map<char, std::string> misMatchTbl = { // should I order values in most to least likely??
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
            {'-', "ACGTMRYKSWBDHV"}
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
    void findMicrosat(const std::string &sequence, int minLenRepeats, int maxLenRepeats, int minLenMicrosat,
                      int mismatchPerc) {

        // Get length of the sequence
        int lenOfSequence = int(sequence.length());


        // Going through all the potential repeat lengths
        for (int lenOfRepeats = minLenRepeats; lenOfRepeats <= maxLenRepeats; lenOfRepeats++) {
            // Going through all bases in the sequence 0 indexed, as microsat can start from any position


            for (int nucleotidePos = 0; nucleotidePos < lenOfSequence; nucleotidePos++) {

                // Check if it's the end of the sequence - will cut off the end of a sequence
                if (nucleotidePos + lenOfRepeats > lenOfSequence) { break; }

                // resetting values
                int startPos = nucleotidePos;
                //float currMismatchPerc = 0;
                // set initial;y to -1 in case correct endPos hasn't been found
                int endPos = -1;
                // check to see if repeats pass the minlenMicrosat threshold fo it to be considered a microsatellite
                int lenThreshold = 0;
                std::string microSat;
                float noMismatches = 0;


                // while first and second repeats equals each other and the mismatch percentage hasn't been crossed
                // compare repeats include comparing non ATGC values
                std::string firstRepeat = sequence.substr(nucleotidePos, lenOfRepeats);
                std::string secondRepeat = sequence.substr(nucleotidePos + lenOfRepeats, lenOfRepeats);
                bool compareAdjRepeats = compareRepeats(firstRepeat, secondRepeat);


                while (compareAdjRepeats or (int(noMismatches / microSat.length()*100) < mismatchPerc)) {

                    // updating mismatch percentage to see if the threshold has been passed
                    // checks before adding new value to microsatllite
                    if (not compareAdjRepeats and not microSat.empty()) {
                        // compares each position in each repeat to see how far the repeats don't match
                        for (int i = 0; i < lenOfRepeats; i++) {
                            if (firstRepeat[i] != secondRepeat[i]) {
                            //if (not compareRepeats(std::to_string(firstRepeat[i]), std::to_string(secondRepeat[i]))) {
                                //increase penalty score for each mismatch by 2
                                penaltyScore += nomatchPenalty;
                                noMismatches++;
                            }
                            // if adding part of a repeat breaks the threshold don't add the repeat to the microsat
                            if ((noMismatches / microSat.length()*100) >= mismatchPerc){
                                // go back nucleotide positions as not adding to microsat
                                nucleotidePos = endPos ;
                                break;
                            }

                        }
                        if ((noMismatches / microSat.length()*100) >= mismatchPerc){break;}
                    }

                    // if the microsat length is empty found pos of the microsat the threshold needs to be increase
                    if (microSat.empty()) {
                        lenThreshold++;
                        microSat = firstRepeat;
                    }
                        // adds the second repeat to the microsat string
                    else{
                        microSat += secondRepeat;
                        lenThreshold++;
                    }


                    // assigned to the end of the second substring
                    endPos = nucleotidePos + lenOfRepeats - 1;

                    // break out of loop if the rest of sequence is not long enough for another comparison
                    if (endPos + lenOfRepeats >= lenOfSequence) {
                        break;
                    }

                    // move onto next adjacent repeats
                    nucleotidePos += lenOfRepeats;
                    //if (nucleotidePos + lenOfRepeats > lenOfSequence){break;}

                    // need to make it find the most common repeat in the microsatellite in case
                    // two microsatellites are next to each other
                    firstRepeat = findMostCommonRepeatInMicoSat(microSat, lenOfRepeats);
                    secondRepeat = sequence.substr(nucleotidePos, lenOfRepeats);
                    compareAdjRepeats = compareRepeats(firstRepeat, secondRepeat);
                }

                // if repeats meet the threshold output the result
                if (lenThreshold >= minLenMicrosat) {
                    std::cout << "Microsatellite found: " << microSat << "\n";
                    // most common repeat is used due to mismatches in the sequence
                    std::string mostCommonRepeat = findMostCommonRepeatInMicoSat(microSat, lenOfRepeats);
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
    static std::string findMostCommonRepeatInMicoSat(const std::string &microsat, int repeatLen) {

        std::unordered_map<std::string, int> counter;
        std::string mostCommonRepeat;
        // maxVal keeps track of the maximum value in the microsat
        int maxVal = -1;

        // counts the number of that specific repeat and adds them to the hashtable
        for (int i = 0; i + repeatLen < microsat.length() ; i += repeatLen) {
            int initialCount = counter[microsat.substr(i, repeatLen)];
            int currentCount = initialCount + 1;
            counter[microsat.substr(i, repeatLen)] = currentCount;
        }

        // Goes through unordered maps of counts to the repeat value in the microsat
        for (auto &[key, val]: counter) {
            if (val > maxVal) {
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
    bool compareRepeats(std::string firstRepeat, std::string secondRepeat) { // both first and second repeat should be the same size
        int lenFirstRepeat = int(firstRepeat.length());
        // Goes through the bases in the first repeat
        for (int base = 0; base < lenFirstRepeat; base++) {
            // if the first base is equal to the second base i.e. A = A
            if (firstRepeat[base] == secondRepeat[base]) {
                continue;
            } else {
                // check if bases are comparative ie. A = N
                std::string mismatches = misMatchTbl[firstRepeat[base]];
                for (int i = 0; i < mismatches.length(); i++) {
                    if (secondRepeat[base] == mismatches[i]) {
                        // add to penalty score if the value had to be 'replaced' ie. A = N
                        penaltyScore += mismatchPenalty;
                        break;
                    }
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
    void addToHashtable(const std::string &repeat, int startPos, int endPos) {

        // If repeat is not in hashtable - if micosat hasn't been found already
        if (hashtable.find(repeat) == hashtable.end()) {
            hashtable[repeat] = {startPos, endPos, penaltyScore};
        }

        // if microsat has already been found in another position
        else {
            std::vector<int> vec = hashtable[repeat];
            // Updating the vector with additional start and stop positions
            vec.push_back(startPos);
            vec.push_back(endPos);
            vec.push_back(penaltyScore);
            hashtable[repeat] = vec;
        }
    }

    /**
     * Print out the hashtable for visual representation
     */

    void printHashTable() {
        // Printing of Unordered_MAP
        std::cout << "HASHTABLE" << std::endl;
        for (auto &[key, vec]: hashtable) {
            std::cout << "Key: " << key << "    " << std::endl;
            std::cout << "Start pos, end pos and penalty score: ";
            for (int i: vec) {
                std::cout << i << "  ";
            }
            std::cout << "\n";
        }
    }

};

int main() {
    // A repeat is the repeat in a micosat i.e. ATGATGATGATGATGATG the repeat = ATG
    std::string sequence;
    int minLenRepeats;
    int maxLenRepeats;
    int minLenMicrosat;
    float mismatchPerc;

    std::cout << "Enter the sequence: ";
    std::cin >> sequence;

    std::cout << "Enter the minimum length of the Microsat: ";
    std::cin >> minLenMicrosat;

    std::cout << "Enter the minimum length of repeats: ";
    std::cin >> minLenRepeats;

    std::cout << "Enter the maximum length of repeats: ";
    std::cin >> maxLenRepeats;

    std::cout << "Enter mismatches percentage allowed: ";
    std::cin >> mismatchPerc;

    MicrosatFinder microsatFinder(2,1);

    microsatFinder.findMicrosat(sequence, minLenRepeats, maxLenRepeats, minLenMicrosat, int(mismatchPerc));

}