#!/bin/bash

#This Skript ist for setting up, the arm64 crossbuild env 

arm64=false 
armhf=true

####################################################
#################### Setup Mirror ##################
####################################################

user="root"
mirrorDebianarm64="http://ftp.debian.org/debian"
mirrorDebianaarmhf="http://raspbian.raspberrypi.com/raspbian/"
targetRelease="bookworm"
packageInchrootARMHF="gcc-arm-linux-gnueabihf,g++-arm-linux-gnueabihf,crossbuild-essential-armhf"
packageInchrootARM64="gcc-aarch64-linux-gnu,g++-aarch64-linux-gnu,crossbuild-essential-arm64"

####################################################
#################### doing ... #####################
####################################################

###################################################################################################################
########################################## Raspberry arm64 ########################################################

if [[ "${arm64}" == true ]]; then
    debootstrap --arch=arm64 --variant=buildd --components=main --include=$packageInchroot \
    $targetRelease /opt/raspberry/arm64 $mirrorDebianarm64 $targetRelease --verbose

#chroot config
cat <<EOF >>/etc/schroot/schroot.conf

[raspi-arm64]
description=chroot raspberry pi
type=directory
directory=/opt/raspberry/arm64
users=root

EOF

  if [[ "${user}" == "root" ]]; then
    echo "/$user/workspace /$user/workspace none rw,bind 0 0" | sudo tee -a /etc/schroot/default/fstab
  else
    echo "/home/$user/workspace /home/$user/workspace none rw,bind 0 0" | sudo tee -a /etc/schroot/default/fstab 
  fi

  echo "/opt/raspberry/arm64 /opt/raspberry/arm64 none rw,bind 0 0" | sudo tee -a /etc/schroot/default/fstab 
fi

###################################################################################################################
########################################### armhf##################################################################

if [[ "${armhf}" == true ]]; then    
    # debootstrap --arch=armhf --variant=buildd --components=main,contrib,rpi,firmware,non-free --no-check-gpg --include=$packageInchroot \
    # $targetRelease /opt/raspberry/armhf $mirrorDebianaarmhf $targetRelease --verbose

    debootstrap --arch=armhf --variant=buildd --components=main --no-check-gpg --include=$packageInchrootARMHF \
    $targetRelease /opt/raspberry/armhf $mirrorDebianaarmhf $targetRelease --verbose

#chroot config
cat <<EOF >>/etc/schroot/schroot.conf

[raspi-armhf]
description=chroot raspberry pi
type=directory
directory=/opt/raspberry/armhf
users=root

EOF

  if [[ "${user}" == "root" ]]; then
    echo "/$user/workspace /$user/workspace none rw,bind 0 0" | sudo tee -a /etc/schroot/default/fstab
  else
    echo "/home/$user/workspace /home/$user/workspace none rw,bind 0 0" | sudo tee -a /etc/schroot/default/fstab 
  fi

  echo "/opt/raspberry/armhf /opt/raspberry/armhf none rw,bind 0 0" | sudo tee -a /etc/schroot/default/fstab 
fi

###################################################################################################################

# install .bashrc
# cp -f ./.vscode/.bashrc /root/.bashrc
# /opt/raspberry/armhf/root/.bashrc 