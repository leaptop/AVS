#!/bin/bash
printf "Дата: "
date --date="today" +%d-%m-%Y 
printf "Имя учётной записи: "
whoami
printf "PC name: "
hostname
