import cgi
import html


# Parse query string
form = cgi.FieldStorage()

# Get username and password from form
name = form.getvalue("name", "")
age = form.getvalue("age", "")

# Print the HTML response
print(f"""<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Login Result</title>
</head>
<body>
    <h1>Login Information</h1>
    <h2>welcome {html.escape(name)}</h2>
    <p>Username: {html.escape(name)}</p>
    <p>age: {html.escape(age)}</p>
</body>
</html>
""")
