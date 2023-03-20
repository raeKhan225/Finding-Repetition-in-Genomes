from flask import Flask, render_template
from flask import request
from werkzeug.utils import secure_filename
from pymongo import MongoClient
from datetime import datetime

app = Flask(__name__, template_folder='template')


@app.route("/")
def homepage():
    return render_template('index.html')


@app.route('/upload', methods=['GET', 'POST'])
def upload_file():
    if request.method == 'POST':
        f = request.files['fastaFile']

        # # https://www.digitalocean.com/community/tutorials/how-to-use-mongodb-in-a-flask-application
        # # Connect to the MongoDB client
        # client = MongoClient('mongodb://localhost:27017/')
        #
        # # Select the database and collection
        # db = client['mydatabase']
        # collection = db['mycollection']
        #
        # # Define the data to insert into the database
        # data = {
        #     'timestamp': datetime.now(),
        #     'project_title': 'Test Project',
        #     'email_address': 'rak12@aber.ac.uk',
        # }
        #
        # # Insert the data into the collection
        # result = collection.insert_one(data)
        #
        # # Print the ID of the inserted document
        # print("Document inserted with ID:", result.inserted_id)

        f.save(f"/Users/raeesahkhan/Documents/uni/Major_Project/web_microsat_finder/uploads/{secure_filename(f.filename)}")
    return homepage() # need to change to redirect to to  / path


if __name__ == '__main__':
    app.run(debug=True)
