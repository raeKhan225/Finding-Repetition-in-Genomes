from flask import jsonify, request
import uuid
import datetime
from application import db


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
            "date_created": datetime.datetime
        }

        db.jobs.insert_one(job)

        return jsonify(job), 200
