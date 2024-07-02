import threading
import requests

url = "http://localhost:80/"
threads = []

def send_request(i):
    try:
        response = requests.get(url)
        print(f"Request {i}: Status Code: {response.status_code}")
    except requests.exceptions.RequestException as e:
        print(f"Request {i}: Failed with exception {e}")

for i in range(0, 1000):
    thread = threading.Thread(target=send_request, args=(i,))
    threads.append(thread)
    thread.start()

for thread in threads:
    thread.join()
