#!/bin/bash

# Check arguments.
# > - redirects output, &2 - efers to file descriptor 2, which is the standard error (stderr) stream.
input_file=""
if [ $# -eq 1 ]; then
    if [ ! -f "$1" ]; then
        echo "Ошибка: Файл '$1' не существует." >&2
        exit 1
    fi
    input_file="$1"
elif [ $# -gt 1 ]; then
    echo "Ошибка: Слишком много аргументов. Используйте только один файл." >&2
    exit 1
else
    echo "Ошибка: Необходимо ввести название файла в качестве параметра." >&2
    exit 1
fi

awk '
BEGIN {
    # 1 - needed, 0 - not
    need_capital = 1
}
{
    line = $0
    
    if (need_capital) {
        if (match(line, /[^[:space:]]/)) {
            # divide string for parts
            before = substr(line, 1, RSTART-1)
            first_char = toupper(substr(line, RSTART, 1))
            rest = substr(line, RSTART+1)
            line = before first_char rest
            need_capital = 0
        }
    }

    offset = 0
    while (1) {
        current_sub = substr(line, offset+1)
        if (match(current_sub, /([.!?])([[:space:]]*)([a-z])/)) {
            pos = offset + RSTART

            # separator like .!?
            sep = substr(line, pos, 1)

            # spaces between separator and first lowercase letter
            spaces = substr(line, pos+1, RLENGTH-2)

            # first lowercase letter after separator
            letter = substr(line, pos+RLENGTH-1, 1)
            
            # reform origin line
            new_sep = sep spaces toupper(letter)
            line = substr(line, 1, pos-1) new_sep substr(line, pos+RLENGTH)
            offset = pos + length(new_sep) - 1
        } else {
            break
        }
    }

    if (match(line, /[.!?][[:space:]]*$/) || (match(line, /^[[:space:]]*$/) && need_capital == 1)) {
        need_capital = 1
    } else {
        need_capital = 0
    }

    print line
}
' "$input_file"