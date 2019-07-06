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

