# plget
A very lightweight handy utility for parsing Apple propery list files (PLIST).


## Preparing for building

*Required dependencies: `libxml2`, `gcc`, `make`*

### Windows
- Install [MSYS2](https://www.msys2.org/)
- Run the MSYS2 environment then proccedd to Install the required packages:
```bash
$ pacman -S gcc \
          git \
          make \
          libxml2-devel
```

### Linux
- Update the packages:
```shell
sudo apt-get update
```
- Install the required packages:
```
sudo apt install \
          gcc \
          git \
          libxml2 \
          libxml2-dev \
          make
```


### MacOS
- Install Xcode command line tools and [Homebrew](https://brew.sh/)
- Then with brew install the required packages:
```
brew install libxml2 automake
```


## Compiling
- There is a crude configure.sh to determine which libxml2 to use. To compile, do
- After you install the required packages, then proceed this way:
```bash
$ git clone https://github.com/kallewoof/plget
$ cd src
$ ./configure.sh
$ make
$ make install # optional -- skip if you don't want a /usr/local/bin/plget binary
```


## Usage

- Example:
```bash
$ cat example/device.plist
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
	<key>UDID</key>
	<string>129FE671-F8D2-446D-9B69-DE56F1AC80B9</string>
	<key>deviceType</key>
	<string>com.apple.CoreSimulator.SimDeviceType.iPad-Retina</string>
	<key>name</key>
	<string>iPad Retina</string>
	<key>runtime</key>
	<string>com.apple.CoreSimulator.SimRuntime.iOS-8-0</string>
	<key>state</key>
	<integer>1</integer>
</dict>
</plist>

$ plget example/device.plist UDID
129FE671-F8D2-446D-9B69-DE56F1AC80B9
$ runtime=`plget example/device.plist runtime`
$ echo The runtime: $runtime
The runtime: com.apple.CoreSimulator.SimRuntime.iOS-8-0
```

* To get a list of device types and runtimes for all devices in the ~/Library/Developer/CoreSimulator/Devices folder, you can do
```bash
$ cd ~/Library/Developer/CoreSimulator/Devices
$ for i in *; do 
devtype=`plget $i/device.plist deviceType`
runtime=`plget $i/device.plist runtime`
devtype=${devtype:38} # trim out "com.apple.CoreSimulator.SimDeviceType."
runtime=${runtime:35} # trim out "com.apple.CoreSimulator.SimRuntime."
echo "$devtype $runtime"
done
```

* which may return e.g.
```bash
iPhone-5s iOS-8-1
iPad-Retina iOS-8-0
iPad-2 iOS-8-1
iPad-Air iOS-8-0
```

* You can use multi component paths:
```bash
$ plget example/applicationState.plist com.apple.mobileslideshow/SBApplicationAllSupportedNotificationTypes
7
```

* You can use the wildcard component * to match all keys. This changes the output to print out all encountered keys (wildcard only) with a * prefix, and will print out multiple (if encountered) values.
```bash
$ plget example/applicationState.plist "*/compatibilityInfo/sandboxPath"
*com.mycompany.myapp
/Users/me/Library/Developer/CoreSimulator/Devices/129FE671-F8D2-446D-9B69-DE56F1AC80B9/data/Containers/Data/Application/69F7E3EF-B450-4840-826D-3830E79C247A
*com.mycompany.otherapp
/Users/me/Library/Developer/CoreSimulator/Devices/129FE671-F8D2-446D-9B69-DE56F1AC80B9/data/Containers/Data/Application/473F8259-EE11-4417-B04E-6FBA7BF2ED05
*com.apple.mobilecal
*com.apple.mobilesafari
*com.apple.mobileslideshow
*org.cocoapods.demo.pajdeg
/Users/me/Library/Developer/CoreSimulator/Devices/129FE671-F8D2-446D-9B69-DE56F1AC80B9/data/Containers/Data/Application/C3069623-D55D-462C-82E0-E896C942F7DE
```
