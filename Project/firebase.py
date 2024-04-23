#-*- coding: utf-8 -*-
import time
import firebase_admin
from firebase_admin import db, credentials

databaseURL = {"databaseURL" : "https://majordb-default-rtdb.firebaseio.com"}
cred = credentials.Certificate("firebase.json")
firebase_admin.initialize_app(cred, databaseURL)

ref = db.reference("/")

db.reference("/count").set(0)

def sendData(random_number):
    db.reference("/").update({"count" : random_number})
