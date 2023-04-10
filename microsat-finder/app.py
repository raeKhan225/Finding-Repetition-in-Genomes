#!/usr/bin/env python
from flask import Flask, render_template

app = Flask(__name__, template_folder='templates/')


@app.route('/')
def home():
    return render_template("home.html")


@app.route('/upload/')
def upload_fasta_file():
    return render_template("upload-FASTA-file.html")


if __name__ == '__main__':
    app.run(debug=True)
