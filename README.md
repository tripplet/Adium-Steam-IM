Adium-Steam-IM
==============
Adium protocol plugin to support Steam instant messaging

[![Build Status](https://travis-ci.org/tripplet/Adium-Steam-IM.svg?branch=master)](https://travis-ci.org/tripplet/Adium-Steam-IM)

Download
========
Download the working version 1.8 [here](https://github.com/tripplet/Adium-Steam-IM/releases/tag/v1.8)
* The precompiled build is for OSX Yosemite (10.9), it might not work under other versions



Thanks
======
* [Pidgin-Opensteamworks](https://code.google.com/p/pidgin-opensteamworks/)
* Old code from http://www.adiumxtras.com/index.php?a=xtras&xtra_id=8339


### How to build yourself
1. Checkout this git repository
2. Init submodules:
   `git submodule init; git submodule update`
5. Compile Adium:
   `cd adium; make`
6. Compile mbedtls:
   `cd mbedtls; mkdir build; cd build; cmake -DCMAKE_OSX_ARCHITECTURES="i386;x86_64" -DENABLE_TESTING=OFF -DENABLE_PROGRAMS=OFF ..; make`
7. Compile the plugin:
   `xcodebuild -configuration Release -project "Steam IM.xcodeproj"`
