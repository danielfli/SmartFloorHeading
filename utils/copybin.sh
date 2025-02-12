#!/bin/bash

cd $1
ip=$2
user=$3

pwd

ssh -o ConnectTimeout=5 $user@$ip exit
if [ $? -ne 0 ]; then
  echo "Error - SSH connection to $ip failed!"
  exit 1
fi

rsync -avh --progress include/*  $user@$ip:/usr/local/include
rsync -avh --progress bin/*  $user@$ip:/usr/local/bin
rsync -avh --progress lib/*  $user@$ip:/usr/local/lib
rsync -avh --progress etc/*  $user@$ip:/usr/local/etc

echo "install success on $ip!"