

#!/bin/bash

# Base URL of the server
BASE_URL="http://localhost:5050"

# Test 1: Request with Disallowed Characters
DISALLOWED_URI="/test?<invalid>"
echo "Testing with disallowed characters in URI:"
curl -v "$BASE_URL$DISALLOWED_URI"
