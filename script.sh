#!/bin/zsh

url="http://localhost/"

# Function to send request and capture status code
send_request() {
    status_code=$(curl -s -o /dev/null -w "%{http_code}" $url)
    echo "Request $1: Status Code: $status_code"
}

# Loop to send 20 requests
for i in {1..20}; do
    send_request $i &
done

# Wait for all background processes to finish
wait
