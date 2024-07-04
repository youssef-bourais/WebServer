#!/bin/bash

batch_size=10  # Adjust batch size for better performance (experiment)
wait_time=0.1  # Adjust wait time as needed (in seconds)

# Define addresses
addresses=(
  "http://localhost/main.cpp"
  # "http://localhost/HtmlPages/minipage/index.html"
  # "http://localhost/ConfiFile/"
)

# Calculate total requests per address
requests_per_address=$((10000 / ${#addresses[@]}))

for ((i=0; i<$requests_per_address; i+=batch_size)); do
  # Send batch of requests for each address
  for address in "${addresses[@]}"; do
    responses=( $(for ((j=i; j<i+batch_size && j<$requests_per_address; ++j)); do curl -s -o /dev/null -w "%{http_code}\n" "$address" & done; wait) )

    # Print status codes for this address
    for response in "${responses[@]}"; do
      echo "$address: $response"
    done
  done

  # Wait before sending next batch
  sleep "$wait_time"
done
