import smtplib
from email.mime.text import MIMEText

import pymongo as pymongo

# Database
from bson import ObjectId
from gridfs import GridFS

client = pymongo.MongoClient('127.0.0.1', 27017)
db = client["microsat_finder_jobs"]
col = db["jobs"]


# Read FASTA file
def read_fasta_file(file):
    seq = ""
    key = ""
    fasta_dict = {}
    #fasta_file_lines = open(file, "r").readlines()
    whole_sequence = ""

    # Tries to read FASTA file
    try:
        # Add FASTA file to dict
        for line in file:
            if line.startswith(">"):
                seq = ""
                key = line[1:].strip()
            else:
                seq += line.strip()

            # replacing old seq with updated
            fasta_dict[key] = seq
        # return fasta_dict

    except Exception as e:
        print("Error when reading FASTA file : " + str(e))

    for key, value in fasta_dict.items():
        whole_sequence += value

    return whole_sequence


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


def sendEmail(email, message_content):
    message = MIMEText(message_content)
    message['From'] = 'rak12@aber.ac.uk'
    message['To'] = email
    message['Subject'] = 'Completed Job microsatellite finder'

    # The email has to be sent on the Aberystwyth University Network
    smtp_conn = smtplib.SMTP('smtp.aber.ac.uk', 25)
    smtp_conn.ehlo()
    smtp_conn.starttls()

    # Convert message to string and send
    smtp_conn.sendmail('rak12@aber.ac.uk', email, message.as_string())

    # Terminate SMTP session
    smtp_conn.quit()


if __name__ == "__main__":
    # Order db from oldest to newest
    sorted_db = col.find({}).sort('date', pymongo.DESCENDING)

    # Get oldest job details
    job = sorted_db[0]
    print(job)

    # Read FASTA file from db
    # Get file from job
    fs = GridFS(db)

    # Find the file's document by _id
    file_id = job.get('fasta_file')
    file_doc = db.fs.files.find_one({"_id": file_id})

    # Retrieve the file's data by reading its chunks
    chunks = db.fs.chunks.find({"files_id": file_id}).sort("n")
    data = b"".join([chunk["data"] for chunk in chunks])

    # Print the file's contents
    print(read_fasta_file(data.decode("utf-8"))) # Error with seq at the begining

    # Execute C++ program

    # Find which sequence the microsat is in

    # Generate GFF3 File

    # Send email to user with their GFF3 file attached

    # Delete from database? Or flag as done on database
