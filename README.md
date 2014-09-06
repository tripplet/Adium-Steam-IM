[![Build Status](https://travis-ci.org/tripplet/Adium-Steam-IM.svg?branch=travis-ci)](https://travis-ci.org/tripplet/Adium-Steam-IM)

Adium-Steam-IM
==============
Adium protocol plugin to support Steam instant messaging

Download
========
Download the working version 1.7.3 [here](https://github.com/tripplet/Adium-Steam-IM/releases/download/v1.7.3/AdiumSteamIM_v173.zip)
* The precompiled build is for OSX Mavericks (10.9), it might not work under other versions

Thanks
======
* Will Ross ([paxswill](https://github.com/paxswill)) for implementing the oppenssl code
* [Pidgin-Opensteamworks](https://code.google.com/p/pidgin-opensteamworks/)
* Old code from http://www.adiumxtras.com/index.php?a=xtras&xtra_id=8339


### How to build
1. Download Adium source-code: https://trac.adium.im/wiki/GettingNewestAdiumSource
2. Compile Adium
3. Compile polarssl -- "cd polarssl; mkdir build; cd build; cmake -DCMAKE_OSX_ARCHITECTURES="i386;x86_64" ..; make"
4. Set the Variable "ADIUM_DEVELOP_PATH" in BuildConfig.xcconfig to your Adium source path
5. Compile the plugin
