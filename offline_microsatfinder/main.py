import subprocess
import sys

scaffold_dict = {}


# Read FASTA file
def read_fasta_file(filecontent):
    # Split by > to get individual scaffolds
    scaffolds = filecontent.split(">")  # Error with seq at the beginning

    for scaffold in scaffolds:
        # If not empty
        if scaffold != '':
            # Split scaffold by first instance on \n to get the scaffold name and value separate
            split_scaffold = scaffold.split("\n", 1)

            # Add to dictionary with actual sting and size and get rid of \t, \n and spaces
            key = split_scaffold[0].replace('\n', '').replace('\t', '').strip()
            value = split_scaffold[1].replace('\n', '').replace('\t', '').strip()

            scaffold_dict[key] = value


"""
A GFF3 file, is

seqid - name of the chromosome or scaffold; chromosome names can be given with or without the 'chr' prefix. Important note: the seq ID must be one used within Ensembl, i.e. a standard chromosome name or an Ensembl identifier such as a scaffold ID, without any additional content such as species or assembly. See the example GFF output below.
source - name of the program that generated this feature, or the data source (database or project name)
type - type of feature. Must be a term or accession from the SOFA sequence ontology
start - Start position of the feature, with sequence numbering starting at 1.
end - End position of the feature, with sequence numbering starting at 1.
score - A floating point value.
strand - defined as + (forward) or - (reverse).
phase - One of '0', '1' or '2'. '0' indicates that the first base of the feature is the first base of a codon, '1' that the second base is the first base of a codon, and so on..
attributes - A semicolon-separated list of tag-value pairs, providing additional information about each feature. Some of these tags are predefined, e.g. ID, Name, Alias, Parent - see the GFF documentation for more details.

must be tab separated

"""


def createGFF(filename):
    try:
        f = open(filename + ".gff3", "x")
        f.write("##gff-version 3\n" +
                "##Microsatellite finder 2023 version 1\n" +
                "##Author: Raeesah Khan\n")
        f.close()
        print("Success, GFF file created")

    except Exception:
        print("Could not create file, file may already exists")
        return False
    return True


def addtoGFF(filename, features):
    try:
        f = open(filename + ".gff3", "a+")
        # tab separated

        f.write(features["seqid"] + "\t" +
                features["source"] + "\t" +
                features["type"] + "\t" +
                features["start"] + "\t" +
                features["end"] + "\t" +
                features["score"] + "\t" +
                features["strand"] + "\t" +
                features["phase"] + "\t" +
                features["attributes"] + "\n")

        f.close()
    except OSError:
        print("Cant read file or file does not exist")


# Stat an end pos have to be 1 indexed
def findSequenceId(start, end):
    counter = 0
    for key, vals in scaffold_dict.items():
        counter += scaffold_dict[key]
        if start < counter:
            return key


if __name__ == "__main__":
    # The program must be called with the FASTA file after the program name
    filename = sys.argv[1]
    file = open(filename, 'r')
    filecontent = file.read()

    minLenMicrosat = input("Enter the minimum length of the microsatellite: ")
    minLenRepeats = input("Enter the minimum length of the repeat: ")
    maxLenRepeats = input("Enter the maximum length of the repeat: ")
    mismatchPerc = input("Enter the mismatch percentage allowed: ")
    projectTitle = input("Enter the project title: ")

    # Execute C++ program
    command = './finding_microsat_perc_threshold_offline/cmake-build-debug/hashTable'
    # print(minLenMicrosat, minLenRepeats, maxLenRepeats, mismatchPerc)

    # Generate GFF3 File
    createGFF(projectTitle)
    microsats_start_stop_penalty_vals = ""

    read_fasta_file(filecontent)
    # Go through sequence in dictionary
    for seqID, sequence in scaffold_dict.items():
        proc = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE)
        output_bytes, _ = proc.communicate(
            input=f"{sequence} {minLenMicrosat} {minLenRepeats} {maxLenRepeats} {mismatchPerc}".encode())

        # Decode the output into a string
        output_str = output_bytes.decode("utf-8")
        microsats_start_stop_penalty_vals = output_str.split('\n')

        for microsat in microsats_start_stop_penalty_vals:
            # Make next dict for featues to be added to gff file
            features = {"seqid": ".", "source": ".", "type": ".", "start": ".", "end": ".", "score": ".",
                        "strand": ".", "phase": ".", "attributes": "."}
            vals = microsat.split('/')
            features["source"] = "Microsatellite finder"
            features["type"] = "Microsatellite"
            features["attributes"] = "Repeat=" + vals[0]

            # 1 to len -2 due to end of vals space and name at the 0th position
            for v in range(1, len(vals) - 2, 3):
                # Have to increase start an end poss by 1 due to 1 index of GFF file
                start = int(vals[v]) + 1
                end = int(vals[v + 1]) + 1
                features["start"] = str(start)
                features["end"] = str(end)
                features["score"] = vals[v + 2]
                features["seqid"] = seqID
                addtoGFF(projectTitle, features)
            print("Job complete")
