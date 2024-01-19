#!/bin/bash
set -v
ls
touch fichfinale
touch fichfinale2
ls | wc
ls > fichfinale2
cat fichfinale2 ; echo "bonjour"
wc -l < fichfinale2