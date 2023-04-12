from flask import Flask, jsonify
from application import db
import uuid

class Job:
    def uploadjob(self, email):
        job = {
            "_id": "",
            "email": email,
            "project_title": "",
            "min_Kmer_length": "",
            "max_Kmer_length": "",
            "min_microsat_length": "",
            "graph_choice": "",
            "fasta_file": ""
        }

        return jsonify(job), 200
