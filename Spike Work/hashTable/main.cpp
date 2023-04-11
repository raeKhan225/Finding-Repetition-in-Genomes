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
    std::unordered_map<std::string, std::vector<int>> hashtable;
    int penaltyScore = 0;

    /**
     * Mismatch table
     */
    // used sting instead of array because of https://stackoverflow.com/questions/21946447/how-much-performance-difference-when-using-string-vs-char-array
    std::unordered_map<char, std::string> misMatchTbl = { // should i order values in most to least likeyly??
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
    void
    findMicrosat(const std::string &sequence, int minLenRepeats, int maxLenRepeats, int minLenMicrosat, float mismatchPerc,
                 std::vector<std::string> scaffoldNames,
                 std::vector<int> scaffoldSizes) {

        // Get length of the sequence
        int lenOfSequence = sequence.length();
        std::string noMismatchRepeat;
        std::string microSat;

        int noMismatches = 0;
        // UGHHHH wanted to do for loops the other way around - AMANDA
        for (int nucleotidePos = 0; nucleotidePos < lenOfSequence; nucleotidePos++) {
            for (int lenOfRepeats = minLenRepeats; lenOfRepeats <= maxLenRepeats; lenOfRepeats++) {
                if (nucleotidePos + lenOfRepeats >= lenOfSequence - 1) { break; }
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

                    std::cout << "Microsatellite found: " << microSat << std::endl;
                    std::string mostCommonRepeat = findMostCommonRepeatInMicoSat(microSat, lenOfRepeats);
                    //addToHashtable(sequence.substr(nucleotidePos, lenOfRepeats), startPos, endPos);
                    addToHashtable(mostCommonRepeat, startPos, endPos);
                }
            }
        }
        printHashTable();

    }

    // looks wrong :((
    void findMicrosatsInscaffolds(std::vector<std::string> scaffoldNames, std::vector<int> scaffoldSizes) {
        std::vector<std::string> foundScaffoldNamesToMicrosat;
        std::vector <std::vector<std::string>> scaffoldNamesToMicrosat ;


        for (auto &[key, vec]: hashtable) {
            int lenCountseq = 0;
            std::vector<std::string>microsatScaffoldpos;
            microsatScaffoldpos.push_back(key);
            for (int i = 0 ; i < vec.size(); i = i + 2) {
                int startPos = vec[i];
                int endPos = vec[i ++];

                for (int scaffoldSizepos = 0; scaffoldSizepos < scaffoldSizes.size(); scaffoldSizepos++) {
                   // if (endPos < lenCountseq){break;}
                    if (startPos <= lenCountseq and endPos >= lenCountseq) {
                        microsatScaffoldpos.push_back(scaffoldNames[scaffoldSizepos]);
                    }
                    lenCountseq += scaffoldSizes[scaffoldSizepos];

                }
            }
            scaffoldNamesToMicrosat.push_back(microsatScaffoldpos);
        }

        for(auto vec: scaffoldNamesToMicrosat){
            for(auto stringVal : vec) {
                std::cout << stringVal << "  ";
            }
            std::cout << "\n";
        }
    }
    /**
     *
     * @param microsat
     * @param repeatLen
     * @return most common repeat in the microsat
     */

    // this function is used after a micosat s found to find the most common repeat in the microsat so that the repeat value can be added to
    // the hashtable accurately instead of it being just the last repeat at the end of the micosat
    static std::string findMostCommonRepeatInMicoSat(const std::string &microsat,
                                                int repeatLen) { // can cause issues when finding repeats and adding to hashtable

        std::unordered_map<std::string, int> counter;
        std::string mostCommonRepeat;

        // counts the number of that specific repeat and adds them to the hashtable
        for (int i = 0; i + repeatLen <= microsat.length() - 1; i += repeatLen) {
            int initialCount = counter[microsat.substr(i, repeatLen)];
            int currentCount = initialCount + 1;
            counter[microsat.substr(i, repeatLen)] = currentCount;
        }

        // Printing of Unordered_MAP
        std::cout << "Counter HASHTABLE" << std::endl;
        for (auto &[key, vec]: counter) {
            std::cout << key << "    " << vec << std::endl;

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
    compareRepeats(std::string firstRepeat, std::string secondRepeat) { // both first and second repeat should be the same size
        int lenFirstRepeat = firstRepeat.length();
        // Goes through the bases in the first repeat
        for (int base = 0; base < lenFirstRepeat; base++) {
            // if the first base is equal to the second base i.e. A = A
            if (firstRepeat[base] == secondRepeat[base]) {
                continue;
            } else {
                std::string mismatches = misMatchTbl[firstRepeat[base]];
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
    void addToHashtable(const std::string &repeat, int startPos, int endPos) {

        // If repeat is not in hashtable - if micosat hasn't been found already
        if (hashtable.find(repeat) == hashtable.end()) {
            hashtable[repeat] = {startPos, endPos};
        }

            // if microsat has already been found in another position
        else {
            std::vector<int> vec = hashtable[repeat];
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
        std::cout << "HASHTABLE" << std::endl;
        for (auto &[key, vec]: hashtable) {
            std::cout << "Key: " << key << "    " << std::endl;
            std::cout << "Start and end positions: ";
            for (int i: vec) {
                std::cout << i << "  ";
            }
            std::cout << "\n";
        }
    }

    void findSequence() {

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

    MicrosatFinder microsatFinder;

    std::vector<std::string> scaffoldNames = {"scaffold_10", "scaffold_1", "scaffold_2", "scaffold_no"};
    std::vector<int> scaffoldSizes = {5, 7, 6, 12};

    microsatFinder.findMicrosat(sequence, minLenRepeats, maxLenRepeats, minLenMicrosat, mismatchPerc, scaffoldNames,scaffoldSizes);
    microsatFinder.findMicrosatsInscaffolds(scaffoldNames, scaffoldSizes);
}