plget
=====

PList parser for shell script processing

There is a crude configure.sh to determine which libxml2 to use. To compile, do
```bash
$ cd src
$ ./configure.sh
$ make
$ make install # optional -- skip if you don't want a /usr/local/bin/plget binary
```

Example:
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

To get a list of device types and runtimes for all devices in the ~/Library/Developer/CoreSimulator/Devices folder, you can do
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

which for me returns
```bash
iPhone-5s iOS-8-1
iPad-Retina iOS-8-0
iPad-2 iOS-8-1
iPad-Air iOS-8-0
iPad-Retina iOS-8-1
iPad-Retina iOS-7-1
Resizable-iPhone iOS-8-0
Resizable-iPhone iOS-8-1
iPhone-5 iOS-7-1
iPad-2 iOS-8-0
iPhone-4s iOS-7-1
iPad-Air iOS-7-1
iPhone-4s iOS-8-1
iPhone-6 iOS-8-0
Resizable-iPad iOS-8-0
iPhone-5s iOS-7-1
iPhone-6 iOS-8-1
iPhone-5s iOS-8-0
iPhone-6-Plus iOS-8-0
iPad-2 iOS-7-1
iPhone-6-Plus iOS-8-1
iPhone-5 iOS-8-1
iPhone-5 iOS-8-0
Resizable-iPad iOS-8-1
iPhone-4s iOS-8-0
iPad-Air iOS-8-1
```
