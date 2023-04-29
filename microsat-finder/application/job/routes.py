from flask import render_template, send_file
# Importing flask app object
from application import app
from application.job.models import Job


# Rendering the upload-FASTA-file.html template through the /upload/ route
@app.route('/upload/')
def upload_fasta_file():
    return render_template("upload-FASTA-file.html")


# This route is run when the upload form is submitted
@app.route("/upload_file/", methods=['POST'])
def upload_file():
    return Job().uploadjob()


# This route is run when the form on the modal is submitted to email users want their current jobs
@app.route("/view_current_jobs/", methods=['POST'])
def get_current_jobs():
    return Job().getjobsfromdb()

# This route is run when the user wants to download the microsat finder
@app.route('/download')
def download_file():
    # path to the zip file to be downloaded
    path = './static/offline_microsatfinder.zip'
    # send the file to the user
    return send_file(path, as_attachment=True)