# send_request.py
#
# This python script sends an HTTP request to the specified IP and resource.
#
# Created By: Doug Sandy
# Creation Date: 4/21/2018
# Version 1.0
#
# Copyright (C) 2018, Arizona State University
# ALL RIGHTS RESERVED
#
import http.client
import sys

# check for the correct number of command line arguments
if len(sys.argv) > 5 or len(sys.argv) < 4:
    print("Improper number of command arguments\n")
    print("Syntax: send_request.py COMMAND IP URI DATA\n")
    exit(0)

cmd = sys.argv[1]
ip = sys.argv[2]
uri = sys.argv[3]
if len(sys.argv) == 5:
    value = sys.argv[4]
else:
    value = ""

if cmd == "PUT":
    # send the PUT request
    s = b'<!DOCTYPE HTML>\r\n<html>\r\n<body>\r\n' + value.encode('utf-8') + b'<br />\r\n</body>\r\n</html>\r\n\r\n'
    conn = http.client.HTTPConnection(ip, 80)
    conn.putrequest(cmd, uri)
    conn.endheaders(s)
    resp = conn.getresponse()
    print(resp.status, resp.reason)
    print(resp.read())
    conn.close()
else:
    # send all other request types
    conn = http.client.HTTPConnection(ip, 80)
    conn.request(cmd, uri)
    resp = conn.getresponse()
    print(resp.status, resp.reason)
    print(resp.read())
