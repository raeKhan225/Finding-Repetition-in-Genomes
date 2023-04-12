from flask import Flask, render_template, request, redirect
from application import app
from application.job.models import Job


@app.route('/upload/test', methods=["GET"])
def upload_test():
    return Job().uploadjob()


@app.route('/upload/')
def upload_fasta_file():
    return render_template("upload-FASTA-file.html")


@app.route("/confirm/", methods=['POST', 'GET'])
def confirm():
    emailAddress = str(request.form["inputEmail"])
    projectTitle = str(request.form["projectTitle"])
    minKmerLength = request.form["minKmerLength"]
    maxKmerLength = request.form["maxKmerLength"]
    minMicrosatLength = request.form["minMicrosatLength"]
    graphType = request.form["graphType"]
    fastaFile = request.files["fastaFile"]


    return render_template("confirmation-page.html", emailAddress=emailAddress, projectTitle=projectTitle,
                           minKmerLength=minKmerLength,
                           maxKmerLength=maxKmerLength, minMicrosatLength=minMicrosatLength, graphType=graphType,
                           fastaFile=fastaFile.filename)


@app.route("/confirm/submit")
def submitJob():
    return render_template('home.html')

@app.route("/uploadingMicroSatDetails/", methods=['POST'])
def uploading_microsat_details():
    return render_template("home.html")


@app.route("/viewCurrentJobs/", methods=["POST"])
def view_current_jobs():
    # run python program to send email to the address
    return redirect('/')
