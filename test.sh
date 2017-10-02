#!/bin/bash

counter=1
while [ $counter -le 100 ]
do
	echo $counter
	nc localhost 1700
	((counter++))
done
