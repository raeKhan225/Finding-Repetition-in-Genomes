import smtplib
import uuid
from datetime import datetime
from email.mime.text import MIMEText
# For uploading files to the database
import gridfs
import pymongo
from bson import json_util
from flask import jsonify, request, json
# Importing db and col from __init__.py in application
from application import db, col


class Job:
    # Upload job to mongodb db
    def uploadjob(self):
        # Create job object
        job = {
            "_id": uuid.uuid4().hex,
            "email": request.form.get('inputEmail'),
            "project_title": request.form.get('projectTitle'),
            "min_Kmer_length": request.form.get('minKmerLength'),
            "max_Kmer_length": request.form.get('maxKmerLength'),
            "min_microsat_length": request.form.get('minMicrosatLength'),
            "perc_mismatch": request.form.get('percMismatch'),
            "fasta_file": "null",
            "date": datetime.today().replace(microsecond=0)
        }

        # Create a new GridFS bucket object
        fs = gridfs.GridFS(db)

        # Get the uploaded file and create a new GridFS file object
        fasta_file = request.files["fastaFile"]
        file_id = fs.put(fasta_file, filename=fasta_file.filename, content_type='text/plain')

        # Update the 'fasta_file' field in the job document with the ID of the new file
        job['fasta_file'] = file_id

        # Insert the job document into the database
        db.jobs.insert_one(job)

        # Return success once completed
        return jsonify("success"), 200

    # Send email to user with their current jobs
    def getjobsfromdb(self):
        # Get email from pop-up
        email = request.form.get('inputcurrjobsEmail')
        result = col.find({"email": email})

        # Get all documents sorted by date in descending order
        cursor = col.find({}).sort('date', pymongo.DESCENDING)
        result_list = list(result)

        # Create message object instance
        message_content = "Current Jobs with microsatellitefinder:\n"

        # Go through each of the jobs associated with the email
        job_id = "null"
        for r in result_list:
            for field, value in r.items():
                if field == "_id":
                    job_id = value
                if field != "email":
                    message_content += str(field) + ": " + str(value) + "\n"

            # Get the order which the job is in the database
            queue_order = 1
            for document in cursor:
                if document['_id'] == job_id:
                    break
                queue_order += 1

            # Add position in queue to email
            message_content += "\n Position in queue: " + str(queue_order) + "\n"

        # Structure email
        message = MIMEText(message_content)
        message['From'] = 'rak12@aber.ac.uk'
        message['To'] = email
        message['Subject'] = 'Current jobs with Microsatellite finder'

        # The email has to be sent on the Aberystwyth University Network
        smtp_conn = smtplib.SMTP('smtp.aber.ac.uk', 25)
        smtp_conn.ehlo()
        smtp_conn.starttls()

        # Convert message to string and send
        smtp_conn.sendmail('rak12@aber.ac.uk', email, message.as_string())

        # Terminate SMTP session
        smtp_conn.quit()

        return json.loads(json_util.dumps(result_list)), 200
