# HeartRateToWeb

## Introduction

This program let you use the heart sensor of your Samsung Watch to display it on software that can read file like OBS. (tutorial below + watch compatibility list below). 

This is a simple python program that react like a mini web server without the need of installing software like WAMP / LAMP or other web server like Apache / Nginix.

## How does it work ?

When you launch the file, the web server will start on your localhost and your LAN IP.

Usually for the best reaction time, you will use the LAN IP on your watch, and the localhost address on your software.

To maximise the compatibility between devices, I started the project on a web app. This is fast and a convinient way to put things in place.

## Bugs ? How do I report one ?

For now, none. If you found one, please use the issue section.

How ? 

 - You must indicate your Platform (Windows / Linux / MacOS) and your version.
 - What you were expecting from the program ?
 - What you actually get from the program ?
 - Computers specs (CPU / RAM / Network Config)


## Compatibility

For now, I know for sure the Samsung Galaxy Watch is compatible, I own one
and it works fine.

Now for the othher watches : 

 - Samsung Galaxy Watch : Yes.
 - Samsung Galaxy Watch Active : must be compatible
 - Samsung Galaxy Watch Active 2 : same as above
 - Samsung Gear Sport : Not sure, but maybe.
 - Samsung Gear S3 : Must be compatible too.
 - Samsung Gear S2 : Must be compatible too.
 - Samsung Gear S : I don't know at all.
 - Watches before this is likely to be too old.
 
Now, watches with the Tizen Plaform version 2.3.2 and + 
is supposed to be compatible.

#### What about the Huawei Watches, Garmin, etc... ?

Well it's complicated, because I don't own thhese watches. I have no clue if you can create your own app like Samsung.

## Releases

You can find release in the release section. There are none for now.
I will wait the watch app to be released first.

## Tutorials

### OBS

There is multiple way to use the heartbeat on OBS, you can choose to read directly from the file, from the heartbeat endpoint 
or the obs endpoint.

#### From file

On OBS, you can add a text source, be sure to select "read from a file", and select the `hr.txt` file in the same directory of your
executable. OBS will take care of refreshing the content automaticly.

#### From /hr endpoint

Only on Windows as far as I know, you can add a Browser source on your OBS, and enter the following
address : `http://localhost:6547/hr` this will output the heartrate at this time.

This is **NOT** refreshed every time.

#### From the /obs endpoint

This is why this project is made, OBS. If you have some knowledge in HTML / CSS and know where to copy/paste code, then
you will be able to make your own template.

The `/obs` endpoint output a HTML page with support of CSS and JS. 
jQuery will be used to refresh the heartrate in real time.

Add a browser source with `http://localost:6547/obs` and OBS will start to show the webpage you created.

## Demos 

https://streamable.com/azuhqn

https://streamable.com/pwtlhf
