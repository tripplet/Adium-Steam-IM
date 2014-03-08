Adium-Steam-IM
==============
Adium protocol plugin to support Steam instant messaging

Download
========
Download the working version 1.7.1 [here](https://github.com/comscandiumplumbumd/Adium-Steam-IM/releases/download/v1.7.1/1.7.1_universal.zip)

Thanks
======
* Will Ross ([paxswill](https://github.com/paxswill)) for implementing the oppenssl code
* [Pidgin-Opensteamworks](https://code.google.com/p/pidgin-opensteamworks/)
* Old code from http://www.adiumxtras.com/index.php?a=xtras&xtra_id=8339


### How to build
1. Download Adium source-code: https://trac.adium.im/wiki/GettingNewestAdiumSource
2. Compile Adium
3. Compile polarssl -- "cd polarssl; mkdir build; cd build; cmake ..; make"
4. Set the Variable "ADIUM_DEVELOP_PATH" in BuildConfig.xcconfig to your Adium source path
5. Compile the plugin
