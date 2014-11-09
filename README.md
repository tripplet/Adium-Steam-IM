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
2. Download Adium: 
   `mkdir adium; cd adium; wget https://bitbucket.org/adium/adium/get/adium-1.5.10.tar.gz`
2. Extract Adium:
   `tar xf adium-1.5.10.tar.gz --strip-components=1;`
3. Compile Adium:
   `xcodebuild -configuration Release -project Adium.xcodeproj`
4. Compile polarssl:
   `cd polarssl; mkdir build; cd build; cmake -DCMAKE_OSX_ARCHITECTURES="i386;x86_64" ..; make`
5. Compile the plugin:
   `xcodebuild -configuration Release -project "Steam IM.xcodeproj"`
