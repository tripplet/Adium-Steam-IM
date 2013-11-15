Adium-Steam-IM
==============
Adium protocol plugin to support Steam instant messaging
Updated version of the Adium Steam Plugin from 
http://www.adiumxtras.com/index.php?a=xtras&xtra_id=8339


How to use precompiled binary
=============================
1. Install Mozilla nss and nspr via homebrew http://brew.sh/ (brew install nss)
2. Download Plugin https://github.com/tripplet/Adium-Steam-IM/releases/download/v1.5/AdiumSteamIM.zip

## **Important**
* The plugin will crash Adium if nss is not properly installed at "/usr/local/Cellar/nss/3.14.1/lib"
* The precpmpiled build is for OSX Mavericks (10.8) on x64, it might not work under other versions


### TODO
* Rewrite steam_rsa.c to use OpenSSL instead of Mozilla NSS, to create platform independent binary build


### How to build
1. Download Adium source-code: https://trac.adium.im/wiki/GettingNewestAdiumSource
2. Compile Adium
3. Install Mozilla nss and nspr via homebrew http://brew.sh/ (brew install nss) 
4. Set the Variable "ADIUM_DEVELOP_PATH" in BuildConfig.xcconfig to your Adium source path
5. Compile the plugin
