#!/usr/bin/env python
from flask import Flask, render_template, request, url_for
import pymongo

app = Flask(__name__, template_folder='templates/')


@app.route('/')
def home():
    return render_template("home.html")


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


@app.route("/uploadingMicroSatDetails/", methods=['POST'])
def uploading_microsat_details():
    return render_template("home.html")


if __name__ == '__main__':
    app.run(debug=True)
