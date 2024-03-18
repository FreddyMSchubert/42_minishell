#!/bin/bash

# takes in a valgrind log file generated with --gen-suppressions=all and extracts the suppressions for specified strings

input_file="$1"
output_file="readline.supp"

search_strings=("readline" "rl_clear_history" "rl_on_new_line" "rl_replace_line" "rl_redisplay" "add_history")

inside_block=0
temp_block=$(mktemp)

trap "rm -f $temp_block" EXIT

check_for_strings() {
	for str in "${search_strings[@]}"; do
		if grep -q "$str" "$temp_block"; then
			return 0 # Found
		fi
	done
	return 1 # Not found
}

while IFS= read -r line || [ -n "$line" ]; do
	if [[ $line == "{" ]]; then
		inside_block=1
		echo "$line" > "$temp_block"
	elif [[ $line == "}" ]]; then
		echo "$line" >> "$temp_block"
		if check_for_strings; then
			cat "$temp_block" >> "$output_file"
		fi
		inside_block=0
	elif [ $inside_block -eq 1 ]; then
		echo "$line" >> "$temp_block"
	fi
done < "$input_file"
