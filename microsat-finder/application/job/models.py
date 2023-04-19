import smtplib
import uuid
from datetime import datetime
from email.mime.text import MIMEText

import gridfs
import pymongo
from flask import jsonify, request

from application import db, col


class Job:

    def uploadjob(self):
        # Create job object
        job = {
            "_id": uuid.uuid4().hex,
            "email": request.form.get('inputEmail'),
            "project_title": request.form.get('projectTitle'),
            "min_Kmer_length": request.form.get('minKmerLength'),
            "max_Kmer_length": request.form.get('maxKmerLength'),
            "min_microsat_length": request.form.get('minMicrosatLength'),
            "graph_choice": request.form.get('graphType'),
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

    def getjobsfromdb(self):
        print("HERE")
        # Get email from pop-up
        email = request.form.get('inputcurrjobsEmail')
        print(email)
        result = col.find({"email": email})
        cursor = col.find({}).sort('date', pymongo.DESCENDING)  # Get all documents sorted by date in descending order

        result_list = list(result)

        # create message object instance
        message_content = "Current Jobs with microsatellitefinder:\n"
        for r in result_list:

            for field, value in r.items():
                if field == "_id":
                    id = value
                if field != "email":
                    message_content += str(field) + ": " + str(value) + "\n"

            queue_order = 1
            for document in cursor:
                if document['_id'] == id:
                    break
                queue_order += 1

            message_content += "\n Position in queue: " + str(queue_order) + "\n"

        message = MIMEText(message_content)
        message['From'] = 'rak12@aber.ac.uk'
        message['To'] = email
        message['Subject'] = 'Current jobs with Microsatellite finder'

        smtp_conn = smtplib.SMTP('smtp.aber.ac.uk', 25)
        smtp_conn.ehlo()
        smtp_conn.starttls()

        # convert message to string and send
        smtp_conn.sendmail('rak12@aber.ac.uk', email, message.as_string())

        # terminate SMTP session
        smtp_conn.quit()

        return jsonify(result_list), 200
