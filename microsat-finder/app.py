from flask import render_template
from application import app
from application.job.routes import *


@app.route('/')
def home():
    return render_template('home.html')


if __name__ == '__main__':
    app.run(debug=True)
