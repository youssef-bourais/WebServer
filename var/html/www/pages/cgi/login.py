import cgi
import html


# Parse query string
form = cgi.FieldStorage()

# Get username and password from form
username = form.getvalue("username", "")
password = form.getvalue("password", "")

# Print the HTML response
print(f"""<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Login Result</title>
</head>
<body>
    <h1>Login Information</h1>
    <h2>welcome {html.escape(username)}</h2>
    <p>Username: {html.escape(username)}</p>
    <p>Password: {html.escape(password)}</p>
</body>
</html>
""")