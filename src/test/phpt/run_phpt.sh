#!/bin/bash

# Exit on on error and on undefined variables
set -o nounset
#set -o errexit

HPHPI="../../../build/src/hphp/hphpi"

function execute_phpt {
    PHPT=$1
    # Parse the phpt
    title=`<$PHPT grep -A 1 -- "--TEST--" | tail -1`
    <$PHPT grep -A 10000 -- "--FILE--" | grep -B 10000 -- "--EXPECT" | tail -n+2 | head -n-1 >file
    <$PHPT grep -A 10000 -- "--EXPECT" | tail -n+2 | sed "s#__FILE__#$PWD/file#" >expected

    echo "Exec test: $title"
    
    # Test the file with HipHop
    $HPHPI -f file >obtained 2>&1

    diff -q obtained expected >/dev/null
    error=$?

    (( error )) && echo "Error on test: $PHPT"

    rm file expected obtained
    return $error
}

tests=0
errors=0

#for file in filter/bug39763.phpt #*/*phpt
if (( $# > 0 ))
then
    target_tests=$1
else
    target_tests=*/*phpt
fi

for file in $target_tests
do
    (( tests++ ))
    execute_phpt $file || (( errors++ ))
done

echo "Executed $tests tests"
echo "Errors: $errors"
