#!/bin/sh
 
echo "Packaging lib in basic format"
 
tar -zcvf mere-message-lib.tgz /usr/local/lib/libmere-message.so* /usr/local/include/mere/message
