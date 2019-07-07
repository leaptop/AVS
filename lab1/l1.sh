#!/bin/bash
printf "Дата: "
date --date="today" +%d-%m-%Y 
printf "Имя учётной записи: "
whoami
printf "PC name: "
hostname
   printf "CPU: \n\tModel:                       "
cat /proc/cpuinfo |  grep --max-count=1 "model name" |awk '{print $4,$5,$6}'
#max-count defines a number of times to repeat the printf with the same line
#awk '{print $4}' - says to print only the word №4
          printf "\tArchitecture:                "
lscpu | grep --max-count=1 "Architecture" | awk '{print $2,$3,$4}'
          printf "\tFrequency:                   "
cat /proc/cpuinfo | grep --max-count=1 "model name" |awk '{print $9}'
          printf "\tNumber of cpu's:             "
lscpu | grep --max-count=1 "CPU(s)" | awk '{print $2,$3,$5}'
	  printf "\tNumber of threads per core:  "
lscpu | grep "Thread(s) per core" | awk '{print $4}'
printf '\n%b\n' "Оператвная память:"
printf   "\tВсего:    "
cat /proc/meminfo | grep "MemTotal" | awk '{print $2,$3}'
printf   "\tДоступно: "
cat /proc/meminfo | grep "MemFree" | awk '{print $2,$3}'
printf '\n%b\n' "Жесткий диск:"
printf "Всего: "
printf  "$(df -H | grep "/dev/sda5" | awk '{print $2}')" #the other way of calling a comand
printf  "\nДоступно: "
printf "$(df -H | grep "/dev/sda5" | awk '{print $4}')"
printf '%b' "\nСмонтированно в корневую директорию /: " #Couldn'tunderstand the task 
printf "$(df -H | grep "/dev/sda5" | awk '{print $6}')"
printf "\nSWAP всего: "
printf  "$(swapon | grep "/dev/" | awk '{print $3}')"
printf "\nSWAP Исспользовано: " #don't know ehere to get Available
printf  "$(swapon | grep "/dev/" | awk '{print $4}')"
printf "\n"
printf '\n%b\n' "Сетевые интрфейсы:"
printf '%b\t' "Имя сетевого интерфейса"
printf '%b\t\t' "MAC адрес"
printf '%b\t' "IP адрес"
printf '%b\n' "Скорость соединения"

#ifconfig | grep ": " | awk '{print $1}' #if colon found(: ) write the first word of the line

for iface in $(ls /sys/class/net/)
do
printf '%b\t' $iface
mac="$(ifconfig $iface | grep "ether" | awk '{print $2}')"
if [[ -n $mac ]]; # -n checks if a string is empty #[[]] allows to use operators and commands
then
        printf '\t\t%b\t' $mac
else
        printf '\t\t%b\t\t\t' "empty"
fi

ip="$(ifconfig $iface | grep "inet " | awk '{print $2}')"
# ip="$(ifconfig $iface | awk '/inet/{print $2}')"
if [[ -n $ip ]];
then
        printf '%b\t' $ip
else
        printf '%b\t\t' "empty"
fi

speed="$(ifconfig $iface | grep -o "txqueuelen [0-9][0-9][0-9][0-9]" | awk '{print $2}')"
if [[ -n $speed ]];
then
        printf '%b\n' $speed
else
        printf '%b\n' "empty"
fi
done
