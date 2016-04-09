# php-5.2_wheezy

This repository provides php-5.2.17 debianized source packages (and additional php modules) patched to be build in Debian 7.x "Wheezy".

## Prerequirement:

You must set up your own debian repository to hold source (binary) debian packages. *packages.YOUROWNREPO.net*

For this task I highly recommend [aptly](http://www.aptly.info/).

## To build you will need:

- Your own debian repo, configured with provided source packages.
- Chroot build environment.
- Correct build and dependency install order (uw-imap, php5, php-modules...)

## Prepare a chroot build environment:

```
apt-get install debootstrap schroot

cat > /etc/schroot/chroot.d/wheezy_amd64.conf <<EOF
[wheezy_amd64]
description=Debian 7 Wheezy amd64
directory=/srv/chroot/wheezy_amd64
root-users=root
type=directory
profile=buildd
personality=linux
preserve-environment=true
EOF

mkdir -p /var/lib/sbuild/build && mkdir -p /srv/chroot/wheezy_amd64
debootstrap --variant=buildd --arch=amd64 --include locales,wget,nano wheezy /srv/chroot/wheezy_amd64 http://ftp.debian.org/debian/

schroot -l
schroot -c wheezy_amd64 -u root --directory /root

sed -i 's/# en_US.UTF-8 UTF-8/en_US.UTF-8 UTF-8/g' /etc/locale.gen && locale-gen

cat <<EOF > /etc/apt/sources.list
deb-src http://mirrors.kernel.org/debian/ wheezy main contrib non-free
deb http://ftp.debian.org/debian wheezy main contrib non-free
deb-src http://ftp.debian.org/debian wheezy main contrib non-free
deb http://ftp.debian.org/debian/ wheezy-updates main contrib non-free
deb-src http://ftp.debian.org/debian/ wheezy-updates main contrib non-free
deb http://security.debian.org/ wheezy/updates main contrib non-free
deb-src http://security.debian.org/ wheezy/updates main contrib non-free
EOF

wget -qO - http://packages.YOUROWNREPO.net/packages.pub | apt-key add -

cat <<EOF > /etc/apt/preferences
Package: *
Pin: release n=wheezy
Pin-Priority: 1100
Package: *
Pin: origin packages.YOUROWNREPO.net
Pin-Priority: 1200
EOF

echo "deb-src http://packages.YOUROWNREPO.net/ wheezy php52" > /etc/apt/sources.list.d/php.list

apt-get update && apt-get -y -t wheezy upgrade && apt-get clean

```

## Build packages using packages.YOUROWNREPO.net:

```
apt-get -y build-dep uw-imap && export DEB_CFLAGS_MAINT_APPEND=-fPIC && apt-get -y --build source uw-imap
dpkg -i libc-client*.deb mlock*.deb # FIX FOR PHP-IMAP
apt-get -y build-dep php5=5.2.17-0+deb7u1 && apt-get -y --build source php5=5.2.17-0+deb7u1
apt-get -y install automake1.4 shtool && dpkg -i php5-dev*.deb php5-common*.deb
apt-get -y build-dep php-apc=3.1.9-0+deb7u1 && apt-get -y --build source php-apc=3.1.9-0+deb7u1
apt-get -y build-dep php-geoip=1.0.8-0+deb7u1 && apt-get -y --build source php-geoip=1.0.8-0+deb7u1
apt-get -y build-dep php-imagick=3.0.1-0+deb7u1 && apt-get -y --build source php-imagick=3.0.1-0+deb7u1
apt-get -y build-dep php-memcache=3.0.6-0+deb7u1 && apt-get -y --build source php-memcache=3.0.6-0+deb7u1
apt-get -y build-dep php-ssh2=0.12-0+deb7u1 && apt-get -y --build source php-ssh2=0.12-0+deb7u1
apt-get -y build-dep php-timezonedb=2015.7-0+deb7u1 && apt-get -y --build source php-timezonedb=2015.7-0+deb7u1
apt-get -y build-dep xdebug=2.0.3-0+deb7u1 && apt-get -y --build source xdebug=2.0.3-0+deb7u1

```

## Unpack sources:

```
dpkg-source -x php5*.dsc
dpkg-source --skip-patches -x php5*.dsc
```

## Prepare original tar archives:

```
tar -C ./ffmpeg-php-0.6.0 -zcvf ffmpeg-php_0.6.0.orig.tar.gz ./ --exclude='./debian'
tar -C ./php5-5.2.17 -zcvf php5_5.2.17.orig.tar.gz ./ --exclude='./debian'
tar -C ./php-apc-3.1.9 -zcvf php-apc_3.1.9.orig.tar.gz ./ --exclude='./debian'
tar -C ./php-geoip-1.0.8 -zcvf php-geoip_1.0.8.orig.tar.gz ./ --exclude='./debian'
tar -C ./php-imagick-3.0.1 -zcvf php-imagick_3.0.1.orig.tar.gz ./ --exclude='./debian'
tar -C ./php-memcache-3.0.6 -zcvf php-memcache_3.0.6.orig.tar.gz ./ --exclude='./debian'
tar -C ./php-ssh2-0.12 -zcvf php-ssh2_0.12.orig.tar.gz ./ --exclude='./debian'
tar -C ./php-timezonedb-2015.7 -zcvf php-timezonedb_2015.7.orig.tar.gz ./ --exclude='./debian'
tar -C ./xcache-1.3.0 -zcvf xcache_1.3.0.orig.tar.gz ./ --exclude='./debian'
tar -C ./xdebug-2.0.3 -zcvf xdebug_2.0.3.orig.tar.gz ./ --exclude='./debian'
```

## Build package:

```
dpkg-buildpackage -us -uc -S [-b]
debuild [--no-tgz-check] -us -uc -S [-b]
```

## Repacking deb package:

```
mkdir -p extract/DEBIAN
dpkg-deb -x package.deb extract/
dpkg-deb -e package.deb extract/DEBIAN
[...do something, e.g. edit the control file...]
mkdir build
sudo chown -R root:root extract/
sudo dpkg-deb -b extract/ build/
```

*Source package formats can be changed in debian/source/format:*

```"3.0     (quilt)"``` *or* ```"3.0      (native)"```
