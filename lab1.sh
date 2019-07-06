#!/bin/bash

printf '%b' "Дата - "
date --date="today" +%F
printf '%b' "Имя учетной записи - "
whoami
printf '%b' "Данное имя ПК - "
hostname

printf '\n%b\n' "Процессор:"
printf '%b' "Модель - "
cat /proc/cpuinfo | grep --max-count=1 "model name" | awk '{print $4,$5,$6,$7}'
printf '%b' "Архитектура - "
lscpu | grep "Архитектура" | awk '{print $2}'
printf '%b' "Тактовая частоата - "
cat /proc/cpuinfo | grep --max-count=1 "model name" | awk '{print $9}'
printf '%b' "Количество ядер - "
lscpu | grep --max-count=1 "CPU(s)" | awk '{print $2,$3,$5}'
printf '%b' "Количество потоков на одном ядре - "
#lscpu | grep "Потоков на ядро" | awk '{print $4}'
lscpu | grep "Thread(s) per core" | awk '{print $4}'

printf '\n%b\n' "Оператвная память:"
printf '%b' "Всего - "
cat /proc/meminfo | grep "MemTotal" | awk '{print $2,$3}'
printf '%b' "Доступно - "
cat /proc/meminfo | grep "MemFree" | awk '{print $2,$3}'

printf '\n%b\n' "Жесткий диск:"
# echo "Файловая система     Размер Использовано  Дост Использовано% Cмонтировано в"
# echo "Файл.система   Размер Использовано  Дост Использовано% Cмонтировано в"
# printf '%b\t' "Файл.система"
# printf '%b\t' "Размер"
# printf '%b\t' "Использовано"
# printf '%b\t' "Дост"
# printf '%b\t' "Использовано%"
# printf '%b\n' "Cмонтировано в"
# df -H | grep "/dev/sda1"
# # swapon
# name="$(swapon | grep "/dev/" | awk '{print $1,$3,$4}')"
# printf '%b\t' $name

printf '%b' "Всего - "
printf '%q\n' "$(df -H | grep "/dev/sda1" | awk '{print $2}')"
printf '%b' "Доступно - "
printf '%q\n' "$(df -H | grep "/dev/sda1" | awk '{print $4}')"
printf '%b' "Смонтированно в корневую директорию / - "
printf '%q\n' "$(df -H | grep "/dev/sda1" | awk '{print $6}')"
printf '%b' "SWAP всего - "
printf '%q\n' "$(swapon | grep "/dev/" | awk '{print $3}')"
swapon | grep "/dev/" | awk '{print $3}'
printf '%b' "SWAP использованно - "
printf '%q\n' "$(swapon | grep "/dev/" | awk '{print $4}')"

printf '\n%b\n' "Сетевые интрфейсы:"
printf '%b\t' "Имя сетевого интерфейса"
printf '%b\t\t' "MAC адрес"
printf '%b\t' "IP адрес"
printf '%b\n' "Скорость соединения"


# ifconfig | grep ": " | awk '{print $1}'
# echo "MAC адрес"
# ifconfig | grep "ether" | awk '{print $2}'
# echo "IP адрес"
# ifconfig | grep "inet " | awk '{print $2}'
# echo "Скорость соединения"
# #ifconfig | grep -o "txqueuelen [0-1000][0-1000][0-1000][0-1000]"

for iface in $(ls /sys/class/net/)
do
printf '%b\t' $iface
mac="$(ifconfig $iface | grep "ether" | awk '{print $2}')"
if [[ -n $mac ]];
then
	printf '\t\t%b\t' $mac
else
	printf '\t\t%b\t\t\t' "nani"
fi

ip="$(ifconfig $iface | grep "inet " | awk '{print $2}')"
# ip="$(ifconfig $iface | awk '/inet/{print $2}')"
if [[ -n $ip ]];
then
	printf '%b\t' $ip
else
	printf '%b\t\t' "nani"
fi

speed="$(ifconfig $iface | grep -o "txqueuelen [0-9][0-9][0-9][0-9]" | awk '{print $2}')"
if [[ -n $speed ]];
then
	printf '%b\n' $speed
else
	printf '%b\n' "nani"
fi

# mac
# cat /sys/class/net/enp2s0/address
# cat /sys/class/net/lo/address 
# cat /sys/class/net/wlp3s0/address
# cat /sys/class/net/$iface/address
# cat /sys/class/net/$iface/tx_queue_len

# ip
# 
# 
# 

# speed
# cat /sys/class/net/enp2s0/tx_queue_len
# cat /sys/class/net/lo/tx_queue_len
# cat /sys/class/net/wlp3s0/tx_queue_len 

# ifconfig | grep "ether" | awk '{print $2}'
# ifconfig | grep "inet " | awk '{print $2}'
# ifconfig | grep -o "txqueuelen [0-1000][0-1000][0-1000][0-1000]"
done
