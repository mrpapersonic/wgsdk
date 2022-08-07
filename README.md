# Discord GameSDK Winamp Plugin

This plugin is largely based off of clandrew's [wdrp](https://github.com/clandrew/wdrp). You can look at the screenshots in that repository, it looks pretty much identical.

### Installing
You'll want to put gen_DiscordGameSDK.dll into Plugins directory, and discord_game_sdk into the Winamp directory.

### Configuration
The config file is located at `%AppData%\Roaming\Winamp\Plugins\wgsdk\config.txt`. It's pretty straightforward how to edit it.

### Differences to wdrp
* uses GameSDK (obviously)
* written in C instead of C++
* uses GNU make instead of Visual Studio
* several other coding differences (e.g. there's no classes in C)

### How to compile
(Disclaimer: This guide is written for Windows users.)

First of all, you want to install the [Winamp SDK](http://forums.winamp.com/showthread.php?t=252090). Now you just need the [Discord GameSDK](https://discord.com/developers/docs/game-sdk/sdk-starter-guide#step-1-get-the-thing).

Then, make sure the repository directory structure looks like this:

```
C:.
├───discord_game_sdk
│   ├───c
│   ├───cpp
│   └───...
└───src
    └───include
```

Now, just run `make`. If you have all the build tools properly installed it should work.
