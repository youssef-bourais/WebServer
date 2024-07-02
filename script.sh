#!/bin/zsh

url="http://localhost/"

# Function to send request and capture response or error
send_request() {
  response=$(curl -s -w "%{http_code}" "$url")
  status_code="${response: -3}"  # Extract the last 3 characters (status code)

  if [[ -z "$response" ]]; then
    echo "Request $1: Error - No response received"
  elif [[ ! $status_code =~ ^[0-9]{3}$ ]]; then
    echo "Request $1: Error - Unexpected response: $response"
  else
    echo "Request $1: Status Code: $status_code"
  fi
}

# Loop to send 20 requests
for i in {1..10000}; do
  send_request $i &
done

# Wait for all background processes to finish
wait

