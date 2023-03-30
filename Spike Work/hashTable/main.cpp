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
     * @param minNoRepeats
     * This is the minimum length of the repeats in the microsatellite
     * @param maxNoRepeats
     * This is the maximum length of the repeats in the microsatellite
     * @param minLenMicrosat
     * This is the minimum length of repeats for the repeats to b considered a microsatellite
     * For example if the minimum length is 3 of repeat ATG then ATGATG will not but considered as a Microsatlite but ATGATGATG will be.
     */

    void findMicrosats(string sequence, int minNoRepeats, int maxNoRepeats, int minLenMicrosat){
        // Get length of the sequence
        int lenOfSequence = sequence.length();

        // Go through each nucleotide in sequence as microsat can start from any position
        for(int nucleotidePos = 0; nucleotidePos <= lenOfSequence - minNoRepeats; nucleotidePos ++){
            for(int lenOfRepeats = minNoRepeats; lenOfRepeats <= maxNoRepeats; lenOfRepeats ++){
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
                }
            }
        }

    }

};

int main() {
    string sequence;
    int minNoRepeats;
    int maxNoRepeats;
    int minLenMicrosat;

    cout << "Enter the sequence: ";
    cin >> sequence;

    cout << "Enter the minimum length of the Microsat: ";
    cin >> minLenMicrosat;

    cout << "Enter the maximum number of repeats: ";
    cin >> maxNoRepeats;

    cout << "Enter the minimum number of repeats: ";
    cin >> minNoRepeats;

    MicrosatFinder microsatFinder;
    microsatFinder.findMicrosats(sequence, minNoRepeats, maxNoRepeats, minLenMicrosat);



}
