# This project is no longer maintained as Adium is no longer actively developed.

Adium-Steam-IM
==============
Adium protocol plugin to support Steam instant messaging

[![CI](https://github.com/tripplet/Adium-Steam-IM/actions/workflows/main.yml/badge.svg)](https://github.com/tripplet/Adium-Steam-IM/actions/workflows/main.yml)
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
   
3. Install openssl (needed for building Adium)

   `brew install openssl cmake`
   
4. Compile Adium:

   `git apply adium.patch; cd adium; make -j`
   
5. Compile mbedtls:

   `cd mbedtls; mkdir build; cd build; cmake -DCMAKE_OSX_ARCHITECTURES="x86_64" -DENABLE_TESTING=OFF -DENABLE_PROGRAMS=OFF ..; make -j`
   
6. Compile the plugin:

   `xcodebuild -configuration Release -project "Steam IM.xcodeproj"`
