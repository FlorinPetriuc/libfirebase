#
# Copyright (C) 2016 Florin Petriuc. All rights reserved.
# Initial release: Florin Petriuc <petriuc.florin@gmail.com>
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License version
# 2 as published by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#


import ctypes
from ctypes import *
firebase = cdll.LoadLibrary("libfirebase.so")

class firebase_Cresponse(ctypes.Structure):
     _fields_ = [("header", ctypes.c_char_p),
                 ("body", ctypes.c_char_p),
                 ("code", ctypes.c_uint16)]

class firebase_PYresponse():
    def __init__(self, code, header, body):
        self.code = code
        self.body = body.decode("utf-8")
        self.full_header = header.decode("utf-8")

        _headers = self.full_header.split('\r\n')
        self.headers = {}
        for h in _headers:
            _h = h.split(': ', maxsplit = 1)
            if len(_h) != 2:
                continue
            self.headers[_h[0]] = _h[1]

def pyfirebase_fcm_send(endpoint, message, endpoint_p256dh, endpoint_auth, api_key):
    _endpoint = ctypes.c_char_p(endpoint.encode('utf-8'))
    _message = ctypes.c_char_p(message.encode('utf-8'))
    _endpoint_p256dh = ctypes.c_char_p(endpoint_p256dh.encode('utf-8'))
    _endpoint_auth = ctypes.c_char_p(endpoint_auth.encode('utf-8'))
    if api_key == 0:
        _api_key = 0
    else:
        _api_key = ctypes.c_char_p(api_key.encode('utf-8'))

    firebase.libfirebase_fcm_send.restype = POINTER(firebase_Cresponse);

    _c_resp = firebase.libfirebase_fcm_send(_endpoint, _message, _endpoint_p256dh, _endpoint_auth, _api_key)

    if not _c_resp:
        return None

    _py_resp = firebase_PYresponse(_c_resp.contents.code, _c_resp.contents.header, _c_resp.contents.body)

    firebase.libfirebase_free_response(_c_resp)

    return _py_resp

def main_file():
    firebase.libfirebase_init(1, ctypes.c_char_p("./python.log".encode('utf-8')))
    r = pyfirebase_fcm_send("https://android.googleapis.com/gcm/send/" +
                            "czoeXf41wbw:APA91bGlXOmAYo5mrIScW_cpMA5t" +
                            "yw5Vt-Ab0lKf3_-1zLW1lukUBTDqUX8sfds-lGka" +
                            "2hXFgDJWj_3I45gEyAmWFP5EAm-b8_OGjLXSLWBy" +
                            "xs9S2XAYvJvARW7vFpbgNUZ-8ilKtAvh",
                            "Hello python people",
                            "BP_1fWolgrIAjT7OXdpNPwELcW2pnB3Ot0sI8kt4SmuBYndgWxqh9zKYVpw9hM4Y0w8orwWr_zAvfyFIIoHUnHY=",
                            "MsoCAf0vLbZRvUYeOIryfA==",
                            "AIzaSyDbfSSZMQNydOeARELbqlRPZ9kja86h5Cc")
    if r is None:
        print("Error sending request")
        return
    print(r.code)
    print(r.full_header)
    print(r.body)
    print(r.headers)

def main():
    firebase.libfirebase_init(1, 0)
    r = pyfirebase_fcm_send("https://android.googleapis.com/gcm/send/" +
                            "czoeXf41wbw:APA91bGlXOmAYo5mrIScW_cpMA5t" +
                            "yw5Vt-Ab0lKf3_-1zLW1lukUBTDqUX8sfds-lGka" +
                            "2hXFgDJWj_3I45gEyAmWFP5EAm-b8_OGjLXSLWBy" +
                            "xs9S2XAYvJvARW7vFpbgNUZ-8ilKtAvh",
                            "Hello python people",
                            "BP_1fWolgrIAjT7OXdpNPwELcW2pnB3Ot0sI8kt4SmuBYndgWxqh9zKYVpw9hM4Y0w8orwWr_zAvfyFIIoHUnHY=",
                            "MsoCAf0vLbZRvUYeOIryfA==",
                            "AIzaSyDbfSSZMQNydOeARELbqlRPZ9kja86h5Cc")
    if r is None:
        print("Error sending request")
        return

    print(r.code)
    print(r.full_header)
    print(r.body)
    print(r.headers)

main()
#main_file()