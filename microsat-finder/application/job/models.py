from flask import jsonify, request
import smtplib
from email.mime.text import MIMEText
import uuid
from datetime import datetime
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
            "fasta_file": request.form.get('fastaFile'),
            "date": datetime.today().replace(microsecond=0)

        }
        # SHOULD CHECK IF JOB ALREADY EXISTS
        db.jobs.insert_one(job)

        return jsonify("Success"), 200

    def getjobsfromdb(self):
        email = request.form.get('inputcurrjobsEmail')
        print(email)
        result = col.find({"email": email})
        result_list = list(result)

        print(result_list)  # print the results to the console

        # create message object instance
        message = MIMEText(str(result_list))
        message['From'] = 'rak12@aber.ac.uk'
        message['To'] = 'tob32@aber.ac.uk'
        message['Subject'] = 'Results for your job search'

        smtp_conn = smtplib.SMTP('smtp.aber.ac.uk', 587)
        smtp_conn.ehlo()
        smtp_conn.starttls()

        # Log in to the server
        smtp_conn.login('rak12', '')



        # convert message to string and send
        smtp_conn.sendmail('rak12@aber.ac.uk', 'tob32@aber.ac.uk', message.as_string())

        # terminate SMTP session
        smtp_conn.quit()
        return jsonify(result_list), 200
