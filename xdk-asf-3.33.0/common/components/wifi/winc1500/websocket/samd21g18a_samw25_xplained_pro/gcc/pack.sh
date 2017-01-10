#!/bin/bash
echo Pre-Built-Step
PATH_TO=../..
gzip -f -k $PATH_TO/www/check.html 
srec_cat $PATH_TO/www/check.html.gz -binary -output $PATH_TO/www/my_page.c -c-Array my_page -include
cp $PATH_TO/www/my_page.c $PATH_TO/
cp $PATH_TO/www/my_page.h $PATH_TO/
