#!/bin/sh
nmap $@ -p 443 | while read line
do
	if [[ $line == 'Nmap scan report for'* ]]; then
		ip=`echo $line |cut -d\( -f2|cut -d\) -f1`
	elif [[ $line == *'tcp open  https' ]]; then
		curl -v -I `echo 'https://'$ip` 2>&1 | while read curline
		do
			if [[ $curline == *'subject: CN='* ]]; then
				echo $ip `echo $curline | cut -d= -f2`
			fi
		done 
	fi
done
