from flask import Flask
import pymongo

app = Flask(__name__)

# Database
client = pymongo.MongoClient('127.0.0.1', 27017)
db = client.job_upload
