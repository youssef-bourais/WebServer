import requests

def chunked_data():
    yield '8\r\n'
    yield 'Mozilla\r\n'
    yield '18\r\n'
    yield 'Developer Network\r\n'
    yield '0\r\n\r\n'

url = 'http://localhost'
headers = {
    'Transfer-Encoding': 'chunked',
    'Content-Type': 'application/octet-stream',
}

response = requests.post(url, data=chunked_data(), headers=headers)
print(response.status_code)
print(response.text)
