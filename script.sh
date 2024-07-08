
#!/bin/zsh

url="http://localhost/"

# Function to send request and capture response or error
send_request() 
{
    # Send GET request
    response=$(curl -s -D - "$url")
    
    # Extract the first line of the response
    first_line=$(echo "$response" | head -n 1)

    # Debug output
    echo "Debug: First Line: $first_line" >&2

    if [[ -z "$first_line" ]]; then
        echo "Request $1: Error - No response received"
    else
        echo "Request $1: $first_line"
    fi
}

# Loop to send requests
for i in {1..100}; do
    send_request $i &
done

# Wait for all background processes to finish
wait

