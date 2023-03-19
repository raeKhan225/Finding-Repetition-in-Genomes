from flask import Flask, render_template
from flask import request
from werkzeug.utils import secure_filename

app = Flask(__name__, template_folder='template')


@app.route("/")
def homepage():
    return render_template('index.html')


@app.route('/upload', methods=['GET', 'POST'])
def upload_file():
    if request.method == 'POST':
        f = request.files['fastaFile']
        f.save(
            f"/Users/raeesahkhan/Documents/uni/Major_Project/web_microsat_finder/uploads/{secure_filename(f.filename)}")
    return homepage()  # need to change to redirect to to  / path


if __name__ == '__main__':
    app.run(debug=True)
