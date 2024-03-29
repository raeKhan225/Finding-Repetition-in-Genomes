def read_fasta_file(file):
    fasta_dict = {}
    fasta_file_lines = open(file, "r").readlines()
    whole_sequence = ""

    # Tries to read FASTA file
    try:
        # Add FASTA file to dict
        for line in fasta_file_lines:
            if line.startswith(">"):
                seq = ""
                key = line[1:].strip()
            else:
                seq += line.strip()

            # replacing old seq with updated
            fasta_dict[key] = seq
        # return fasta_dict

    except Exception as e:
        print("Error when reading FASTA file : " + e)

    for key, value in fasta_dict.items():
        whole_sequence += value

    return whole_sequence



print(read_fasta_file("/Users/raeesahkhan/Documents/uni/Major_Project/Tests/01_test.fa"))