import subprocess
import os
import re
import sys

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

print('\nStarting the webserver..')
webserver = subprocess.Popen(['./webserver', '8080'])

###################################################################

print('---------------------------------')
print("Test 1: Normal HTML file Test")
sys.stdout.write('Creating test1.html')
print('\n---------------------------------')
wr = open('../website/test1.html', 'w')
test1 = '<!DOCTYPE html>\r\n\
<html>\r\n\
<body>\r\n\
<p>What is life</p>\r\n\
</body>\r\n\
</html>'
wr.write(test1)
wr.close()

request = 'curl -i -H "Content-Type: text/html" -X GET http://localhost:8080/test1.html'
telnet_proc = subprocess.Popen(request, stdout=subprocess.PIPE, shell=True)

response = ''
while True:
    line = telnet_proc.stdout.read().decode('utf-8')
    response += line
    if line == '' and telnet_proc.poll() != None:
        break;

expected_response = re.search(r'HTTP/1.1 200 OK(.*)'+re.escape(test1), response, re.DOTALL)
print(response)
if expected_response:
    print bcolors.OKGREEN + '\nTest 1 passed\n' + bcolors.ENDC
else:
    print bcolors.WARNING + '\nTest 1 failed' + bcolors.ENDC

###################################################################

print('---------------------------------')
print("Test 2: Subdirectory Test")
sys.stdout.write('Creating /test2/test2.html')
print('\n---------------------------------')
test2_file = '../website/test2/test2.html'
if not os.path.exists(os.path.dirname(test2_file)):
    os.makedirs(os.path.dirname(test2_file))
wr = open(test2_file, 'w')

wr.write(test1)
wr.close()

request = 'curl -i -H "Content-Type: text/html" -X GET http://localhost\
:8080/test2/test2.html'
telnet_proc = subprocess.Popen(request, stdout=subprocess.PIPE, shell=True)

response = ''
while True:
    line = telnet_proc.stdout.read().decode('utf-8')
    response += line
    if line == '' and telnet_proc.poll() != None:
        break;

#Uses same string as test 1
expected_response = re.search(r'HTTP/1.1 200 OK(.*)'+re.escape(test1), response, re.DOTALL)
print(response)
if expected_response:
    print bcolors.OKGREEN + '\nTest 2 passed\n' + bcolors.ENDC
else:
    print bcolors.WARNING + '\nTest 2 failed' + bcolors.ENDC




webserver.kill()
