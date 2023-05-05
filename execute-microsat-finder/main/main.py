import os
import smtplib
import subprocess
from email import encoders
from email.mime.base import MIMEBase
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
import pymongo as pymongo

# Database
from gridfs import GridFS

client = pymongo.MongoClient('127.0.0.1', 27017)
db = client["microsat_finder_jobs"]
col = db["jobs"]
scaffold_dict = {}


def get_scaffold_dict():
    return scaffold_dict


# Read FASTA file
def read_fasta_file(filecontent):
    # Split by > to get individual scaffolds
    scaffolds = filecontent.split(">")  # Error with seq at the begining

    for scaffold in scaffolds:
        # If not empty
        if scaffold != '':
            # Split scaffold by first instance on \n to get the scaffold name and value seperate
            split_scaffold = scaffold.split("\n", 1)

            # Add to dictionary with actual sting and size and get rid of \t, \n and spaces
            key_val = split_scaffold[0].split(' ')
            key = key_val[0].replace('\n', '').replace('\t', '').replace('\r', '').strip()
            value = split_scaffold[1].replace('\n', '').replace('\t', '').replace('\r', '').strip()

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
        print("Success")

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


# Start and end pos have to be 1 indexed
def findSequenceId(start, end):
    counter = 0
    for key, vals in scaffold_dict.items():
        counter += scaffold_dict[key]
        if start < counter:
            return key


def sendEmail(email, message_content, f):
    message = MIMEMultipart()
    message['From'] = 'rak12@aber.ac.uk'
    message['To'] = email
    message['Subject'] = 'Completed Job microsatellite finder'

    # The email has to be sent on the Aberystwyth University Network
    smtp_conn = smtplib.SMTP('smtp.aber.ac.uk', 25)
    smtp_conn.ehlo()
    smtp_conn.starttls()

    with open(f, 'rb') as attachment:
        part = MIMEBase('application', 'octet-stream')
        part.set_payload(attachment.read())
        encoders.encode_base64(part)
        part.add_header('Content-Disposition',
                        'attachment', filename=f.split("/")[-1])
        message.attach(part)

    message.attach(MIMEText(message_content))

    # Convert message to string and send
    smtp_conn.sendmail('rak12@aber.ac.uk', email, message.as_string())

    # Terminate SMTP session
    smtp_conn.quit()


if __name__ == "__main__":
    # Order db from oldest to newest
    sorted_db = col.find({}).sort('date')

    # Get oldest job details
    job = sorted_db[0]
    document_id = job.get('_id')
    minLenMicrosat = job.get('min_microsat_length')
    minLenRepeats = job.get('min_Kmer_length')
    maxLenRepeats = job.get('max_Kmer_length')
    mismatchPerc = job.get('perc_mismatch')
    projectTitle = job.get('project_title')
    email = job.get('email')

    # Read FASTA file from db
    # Get file from job
    fs = GridFS(db)

    # Find the file's document by _id
    file_id = job.get('fasta_file')
    file_doc = db.fs.files.find_one({"_id": file_id})

    # Retrieve the file's data by reading its chunks
    chunks = db.fs.chunks.find({"files_id": file_id}).sort("n")
    data = b"".join([chunk["data"] for chunk in chunks])

    file_content = data.decode("utf-8")
    read_fasta_file(file_content)

    # Execute C++ program
    command = '/Users/raeesahkhan/Documents/uni/Major_Project/execute-microsat-finder/finding_microsat_perc_threshold/cmake-build-debug/hashTable'
    # print(minLenMicrosat, minLenRepeats, maxLenRepeats, mismatchPerc)

    # Generate GFF3 File
    createGFF(projectTitle)
    microsats_start_stop_penalty_vals = ""
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

    # Send email to user with their GFF3 file attached
    sendEmail(email,
              "Completed Job, please see attached file fo results of your job. Please Note you results are not saved",
              projectTitle + ".gff3")

    # Remove file
    os.remove(projectTitle + ".gff3")

    # remove job from database
    col.delete_one({"_id": document_id})
    db.fs.files.delete_one({"_id": file_id})
