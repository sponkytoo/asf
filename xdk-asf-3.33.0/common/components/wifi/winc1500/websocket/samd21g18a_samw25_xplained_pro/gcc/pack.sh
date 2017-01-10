#!/bin/bash
echo Pre-Built-Step
PATH_TO=../..
gzip -f -k $PATH_TO/www/check.html 
srec_cat $PATH_TO/www/check.html.gz -binary -output $PATH_TO/www/my_page.c -c-Array my_page -include
cp $PATH_TO/www/my_page.c $PATH_TO/
cp $PATH_TO/www/my_page.h $PATH_TO/
rm $PATH_TO/www/check.html.gz
rm $PATH_TO/www/my_page.c
rm $PATH_TO/www/my_page.h
gzip -f -k $PATH_TO/www/jquery.js
srec_cat $PATH_TO/www/jquery.js.gz -binary -output $PATH_TO/www/jquery.c -c-Array jquery -include
cp $PATH_TO/www/jquery.c $PATH_TO/
cp $PATH_TO/www/jquery.h $PATH_TO/
rm $PATH_TO/www/jquery.js.gz
rm $PATH_TO/www/jquery.c
rm $PATH_TO/www/jquery.h

