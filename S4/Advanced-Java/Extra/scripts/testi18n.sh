#!/bin/bash

# Acknowledgements to Dave Dopson for this:
# https://stackoverflow.com/questions/59895/how-do-i-get-the-directory-where-a-bash-script-is-located-from-within-the-script

SOURCE=${BASH_SOURCE[0]}
while [ -L "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
  DIR=$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )
  SOURCE=$(readlink "$SOURCE")
  [[ $SOURCE != /* ]] && SOURCE=$DIR/$SOURCE # if $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
done
DIR=$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )

LIB=$DIR/../../java-advanced-2024/lib/*:bin:$DIR/../java-solutions/info/kgeorgiy/ja/eremin/i18n/test/resources:$DIR/../java-solutions

SRC="$(find "$DIR/../java-solutions/info/kgeorgiy/ja/eremin/i18n/" -type f -name "*.java")"
UTIL="$(find "$DIR/../java-solutions/info/kgeorgiy/ja/eremin/util/" -type f -name "*.java")"

javac -d "$DIR/bin" -cp "$LIB" $UTIL $SRC

java -cp "$LIB" info.kgeorgiy.ja.eremin.i18n.test.Tester