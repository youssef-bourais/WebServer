from datetime import datetime

def print_current_time():
    now = datetime.now()
    current_time = now.strftime("%Y-%m-%d %H:%M:%S")
    print("Current Date and Time: ", current_time)

if __name__ == "__main__":
    print_current_time()
