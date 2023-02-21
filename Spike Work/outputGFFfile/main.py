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
        f = open(filename + ".gff", "x")
        f.write("##gff-version 3\n" +
                "##Microsatellite finder 2023 version 1\n" +
                "##Author: Raeesah Khan\n")
        f.close()
        print("Success")

    except Exception:
        print("Could not create file, file may already exists")
        return False
    return True


def addtoGFF(filename, features):
    try:
        f = open(filename + ".gff", "a+")
        # tab separated
        for feature in features:
            f.write(str(feature["seqid"]) + "\t" +
                    str(feature["source"]) + "\t" +
                    str(feature["type"]) + "\t" +
                    str(feature["start"]) + "\t" +
                    str(feature["end"]) + "\t" +
                    str(feature["score"]) + "\t" +
                    str(feature["strand"]) + "\t" +
                    str(feature["phase"]) + "\t" +
                    str(feature["attributes"]) + "\n")

        f.close()

    except OSError:
        print("Cant read file or file does not exist")


# need to test if file already exist to see if it catches the right error.
# may want to allow user to specify project title and who ran the analysis when the file is inititalised
features = [
    {'seqid': 'chr1', 'source': 'example', 'type': 'gene', 'start': 1000, 'end': 2000, 'score': '.', 'strand': '+',
     'phase': '.', 'attributes': "ID=gene1;Name=exampleGene1;Note=This is an example gene."},

    {'seqid': 'chr1', 'source': 'example', 'type': 'mRNA', 'start': 1200, 'end': 1800, 'score': '.', 'strand': '+',
     'phase': '.', 'attributes': "ID=gene2;Name=exampleGene2;Note=This is an example gene."},

    {'seqid': 'chr1', 'source': 'example', 'type': 'exon', 'start': 1200, 'end': 1300, 'score': '.', 'strand': '+',
     'phase': '.', 'attributes': "ID=gene3;Name=exampleGene3;Note=This is an example gene."},

    {'seqid': 'chr1', 'source': 'example', 'type': 'exon', 'start': 1700, 'end': 1800, 'score': '.', 'strand': '+',
     'phase': '.', 'attributes': "ID=gene4;Name=exampleGene4;Note=This is an example gene."}
]

result = createGFF("test")
if result:
    addtoGFF("test", features)
else:
    print("could not add values to file - file already exists")