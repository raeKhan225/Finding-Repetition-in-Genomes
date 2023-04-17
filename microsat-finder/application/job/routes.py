from flask import Flask, render_template, request, redirect
from application import app
from application.job.models import Job


@app.route('/upload/')
def upload_fasta_file():
    return render_template("upload-FASTA-file.html")


@app.route("/upload/file/", methods=['POST'])
def upload_file():
    return Job().uploadjob()


@app.route("/view_current_jobs/", methods=['POST'])
def get_current_jobs():

    return Job().getjobsfromdb()

