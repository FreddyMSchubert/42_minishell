import sys

def process_file(filename, keywords):
    flag_brace_detected = False
    flag_keyword_found = False
    current_line_number = 0
    start_line_number = 0

    try:
        with open(filename, 'r') as file:
            for line in file:
                current_line_number += 1
                if '{' in line:
                    flag_brace_detected = True
                    flag_keyword_found = False
                    start_line_number = current_line_number

                if flag_brace_detected:
                    if any(keyword in line for keyword in keywords):
                        flag_keyword_found = True

                if '}' in line:
                    if flag_brace_detected and not flag_keyword_found:
                        print(f"Keyword not found between braces starting at line {start_line_number} and ending at line {current_line_number}")
                    flag_brace_detected = False
                    flag_keyword_found = False

    except FileNotFoundError:
        print(f"Error: The file {filename} does not exist.")
    except Exception as e:
        print(f"An error occurred: {str(e)}")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python script.py filename")
    else:
        keywords = ['readline', 'add_history', '/usr/bin/ls']
        process_file(sys.argv[1], keywords)
