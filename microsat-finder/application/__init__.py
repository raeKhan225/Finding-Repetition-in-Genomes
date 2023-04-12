from flask import Flask, render_template
import pymongo

app = Flask(__name__)
# Database
client = pymongo.MongoClient('localhost', 27017)
db = client.user_login_system_test

