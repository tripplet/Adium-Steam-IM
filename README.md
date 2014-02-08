Adium-Steam-IM
==============
Adium protocol plugin to support Steam instant messaging
Updated version of the Adium Steam Plugin from 
http://www.adiumxtras.com/index.php?a=xtras&xtra_id=8339


Download
========
Download the working version 1.7 [here](https://github.com/tripplet/Adium-Steam-IM/releases/download/v1.7/AdiumSteamIM_v17.zip)
* The precompiled build is for OSX Mavericks (10.9) on x64, it might not work under other versions

Thanks
======
* Will Ross ([paxswill](https://github.com/paxswill)) for implementing the oppenssl code
* [Pidgin-Opensteamworks](https://code.google.com/p/pidgin-opensteamworks/)


### How to build
1. Download Adium source-code: https://trac.adium.im/wiki/GettingNewestAdiumSource
2. Compile Adium
3. Compile polarssl -- "cd polarssl; mkdir build; cd build; cmake ..; make"
4. Set the Variable "ADIUM_DEVELOP_PATH" in BuildConfig.xcconfig to your Adium source path
5. Compile the plugin



[![Bitdeli Badge](https://d2weczhvl823v0.cloudfront.net/tripplet/adium-steam-im/trend.png)](https://bitdeli.com/free "Bitdeli Badge")

