#include <iostream>
#include <unordered_map>

class MicrosatFinder {
public:


    //Constructor
    MicrosatFinder(int nomatchPenalty, int mismatchPenalty) {


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

    MicrosatFinder microsatFinder();
//    std:: cout << microsatFinder.compareRepeats("ATG", "ATG") << "\n";



}