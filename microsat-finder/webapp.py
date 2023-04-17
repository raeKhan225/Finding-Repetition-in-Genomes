

from flask import render_template
from application.job.routes import *
from application import app


@app.route('/')
def home():
    return render_template('home.html')


if __name__ == '__main__':
    app.run(debug=True)
