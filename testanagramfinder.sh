#!/bin/bash

file=anagramfinder.cpp
MAXTIME="8.0"

if [ ! -f "$file" ]; then
    echo -e "Error: File '$file' not found.\nTest failed."
    exit 1
fi

num_right=0
total=0
valgrind_deduction=0
line="________________________________________________________________________"
compiler=
interpreter=
language=
extension=${file##*.}
if [ "$extension" = "py" ]; then
    if [ ! -z "$PYTHON_PATH" ]; then
        interpreter=$(which python.exe)
    else
        interpreter=$(which python3.6)
    fi
    command="$interpreter $file"
    echo -e "Testing $file\n"
elif [ "$extension" = "java" ]; then
    language="java"
    command="java ${file%.java}"
    echo -n "Compiling $file..."
    javac $file
    echo -e "done\n"
elif [ "$extension" = "c" ] || [ "$extension" = "cpp" ]; then
    language="c"
    command="./${file%.*}"
    echo -n "Compiling $file..."
    results=$(make 2>&1)
    if [ $? -ne 0 ]; then
        echo -e "\n$results"
        exit 1
    fi
    echo -e "done\n"
fi

timeout() {
    time=$1

    # start the command in a subshell to avoid problem with pipes
    # (spawn accepts one command)
    cmd="/bin/bash -c \"$2\""

    { expect -c "set echo \"-noecho\"; set timeout $time; spawn -noecho $cmd; expect timeout { exit 1 } eof { exit 0 }"; } >/dev/null

    if [ $? -eq 1 ]; then
        return 0
    fi
    return 1
}

run_test_args() {
    (( ++total ))
    echo -n "Running test $total..."
    expected=$2
    local ismac=0
    date --version >/dev/null 2>&1
    if [ $? -ne 0 ]; then
       ismac=1
    fi
    local start=0
    if (( ismac )); then
        start=$(python -c 'import time; print time.time()')
    else
        start=$(date +%s.%N)
    fi
    if timeout $MAXTIME "$command $1 2>&1 | tr -d '\r' > tmp.txt"; then
        echo "failure [timed out after $MAXTIME seconds]"
    else
        received=$(cat tmp.txt)
        { $command $1 2>&1 > /dev/null; } 1>/dev/null 2>error.txt
        error=$(cat error.txt)
        if [ ! -z "$error" ]; then
            received="${received}\n$error"
        fi
        local end=$(date +%s.%N)
        if (( ismac )); then
            end=$(python -c 'import time; print time.time()')
        else
            end=$(date +%s.%N)
        fi
        local elapsed=$(echo "scale=3; $end - $start" | bc | awk '{printf "%.3f", $0}') 
        if [ "$expected" != "$received" ]; then
            echo -e "failure\n\nExpected$line\n$expected\n"
            echo -e "Received$line\n$received\n"
        else
            echo "success [$elapsed seconds]"
            (( ++num_right ))
        fi
    fi
    rm -f tmp.txt error.txt
}

run_test_args "" "Usage: ./anagramfinder <dictionary file>"
run_test_args "file1.txt file2.txt" "Usage: ./anagramfinder <dictionary file>"
run_test_args "nofile.txt" "Error: File 'nofile.txt' not found."

(cat << EOF
EOF
) > words.txt
run_test_args "words.txt" "No anagrams found."
rm -f words.txt

(cat << EOF
a
b
c
d
aa
bb
cc
dd
EOF
) > words.txt
run_test_args "words.txt" "No anagrams found."
rm -f words.txt

(cat << EOF
a
b
c
a
c
d
EOF
) > words.txt
run_test_args "words.txt" $'Max anagram length: 1\na\na\n\nc\nc'
rm -f words.txt

(cat << EOF
a-b-c-d's
b
c
d
abcd---'s
aa
bb
cc
dd
s'd-c-b-a
EOF
) > words.txt
run_test_args "words.txt" $'Max anagram length: 9\na-b-c-d\'s\nabcd---\'s\ns\'d-c-b-a'
rm -f words.txt

(cat << EOF
tea
home
eat
ate
react
trace
EOF
) > words.txt
run_test_args "words.txt" $'Max anagram length: 5\nreact\ntrace'
rm -f words.txt

(cat << EOF
cater
tea
home
eat
ate
react
trace
EOF
) > words.txt
run_test_args "words.txt" $'Max anagram length: 5\ncater\nreact\ntrace'
rm -f words.txt

(cat << EOF
CATER
tea
home
eat
ate
REACT
trace
EOF
) > words.txt
run_test_args "words.txt" $'Max anagram length: 5\nCATER\nREACT\ntrace'
rm -f words.txt

(cat << EOF
pots
stop
tops
a
b
c
eaT
Ate
traCe
tea
d
E
f
reacT
CaTer
EOF
) > words.txt
run_test_args "words.txt" $'Max anagram length: 5\nCaTer\nreacT\ntraCe'
rm -f words.txt

(cat << EOF
pots
REACT
stop
toPs
a
lithonephrotomies
b
cater
nephrolithotomies
EOF
) > words.txt
run_test_args "words.txt" $'Max anagram length: 17\nlithonephrotomies\nnephrolithotomies'
rm -f words.txt

echo -e "\nTotal tests run: $total"
echo -e "Number correct : $num_right"
echo -n "Percent correct: "
echo "scale=2; 100 * $num_right / $total" | bc

if [ "$language" = "java" ]; then
    echo -e -n "\nRemoving class files..."
    rm -f *.class
    echo "done"
elif [ $language = "c" ]; then
    echo -e -n "\nCleaning project..."
    make clean > /dev/null 2>&1
    echo "done"
fi
