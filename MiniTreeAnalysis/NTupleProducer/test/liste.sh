#!/bin/bash

FILE=$1

echo for $FILE

cat $FILE | sed -e 's/^/mydic\=/' > dictdata.py

./liste.py





