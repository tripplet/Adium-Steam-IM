Adium-Steam-IM
==============
Adium protocol plugin to support Steam instant messaging

[![Build Status](https://travis-ci.org/tripplet/Adium-Steam-IM.svg?branch=master)](https://travis-ci.org/tripplet/Adium-Steam-IM)
[![Github All Releases](https://img.shields.io/github/downloads/tripplet/Adium-Steam-IM/total.svg)](https://github.com/tripplet/Adium-Steam-IM/releases)

Download
========
Get the latest version [here](https://github.com/tripplet/Adium-Steam-IM/releases/)

Thanks
======
* [Pidgin-Opensteamworks](https://github.com/eionrobb/pidgin-opensteamworks)
* Old code from http://www.adiumxtras.com/index.php?a=xtras&xtra_id=8339


### How to build yourself
1. Checkout this git repository
2. Init submodules:
   `git submodule update --init`
3. Compile Adium:
   `cd adium; make`
4. Compile mbedtls:
   `cd mbedtls; mkdir build; cd build; cmake -DCMAKE_OSX_ARCHITECTURES="i386;x86_64" -DENABLE_TESTING=OFF -DENABLE_PROGRAMS=OFF ..; make`
5. Compile the plugin:
   `xcodebuild -configuration Release -project "Steam IM.xcodeproj"`
