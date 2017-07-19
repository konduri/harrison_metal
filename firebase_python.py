#!/usr/bin/python

"""
Firebase tool testing for Harrison metal course

Objective is to use REST API's provided by firebase
to send data and receive data to and from a cloud.

Writing a simple python wrapper to put these commands in
"""

import json
import requests

def send_request():
    payload = json.dumps({ "person_1": "Vamshi", "person_2" : "Ishaan" , "what" : "why"})
    r = requests.put('https://test-project-e2100.firebaseio.com/sample.json',
          data=payload)
    return r

def get_request():
    r = requests.get('https://test-project-e2100.firebaseio.com/sample.json')
    return r

import IPython
IPython.embed()
