# SOCOM: FireTeam Bravo 2 (EU) PPSSPP boot fix

The European version of *SOCOM: FireTeam Bravo 2* (UCES00543) has an issue with PPSSPP.  
During startup, the game’s boot loader executes several graphics-related functions within its callback thread.  
These calls somehow interfere with PPSSPP, preventing the game from launching correctly.

This plugin resolves the issue by patching the loader to disable those problematic graphics calls.  
For reference, even with the original calls in place, the loader does not render any visible output.

I highly recommend to use the US version of the game for infrastructure online play.  
The game will (soon) no longer be cross-region between the US/EU versions on the PS Rewired server.

## Build

PSPSDK & toolchain must be installed beforehand.  
Run this in a Bash terminal:
```bash
# Clone this repo with its sub-modules
git clone --recurse-submodules https://github.com/Linblow/ftb2euBootFixPlugin.git

# Build everything
cd ftb2euBootFixPlugin
make dist
```
You will find the generated files and distribution archive in the `dist` sub-directory.  
You can also find the pre-built project in the [Releases](https://github.com/Linblow/ftb2euBootFixPlugin/releases) page.

#### Quick installation after building

If you built the project yourself, you can directly install the files to PPSSPP:
1. Set the Makefile's `ppsspp_memstick_dir` variable to the root of your PPSSPP memstick folder.  
   For instance, when built under WSL, with PPSSPP installed in Windows:
   ```Makefile
   ppsspp_memstick_dir = /mnt/c/Users/<USERNAME>/Documents/PPSSPP
   ```
   With `<username>` being your username.
2. Run this in a Bash terminal:
   ```bash
   make install
   ```
3. Done.

## Installation

Make sure to use the [latest version of the PPSSPP emulator](https://www.ppsspp.org/download/).  
This plugin will only work under the emulator (otherwise, it will exit earlier).  
In either case, it will unload itself automatically when it's done.

### 1. Extract the plugin
Unzip the contents of the distribution archive ie. `dist/bootFixFtb2EuPlugin.zip`.  
You can also find it in the [Releases](https://github.com/Linblow/ftb2euBootFixPlugin/releases) page.

Extract all files to the root of your **PPSSPP memstick** folder.  
Typical memstick location:  
```
C:\Users\<username>\Documents\PPSSPP
```

### 2. Enable plugins in PPSSPP
Make sure plugins are enabled in your PPSSPP configuration file:  
- Navigate to: `PSP/SYSTEM/ppsspp.ini` (inside your memstick folder)  
- Under the `[General]` section, find the line:  
  ```
  EnablePlugins = True
  ```  
  If it doesn’t exist or is set to `False`, change it to:  
  ```
  EnablePlugins = True
  ```
- Save the file.

Note PPSSPP must be completely closed before editing the configuration file 
(or changes will be discarded).

### 3. Done, enjoy

## Credits

[printf library](https://github.com/eyalroz/printf) by eyalroz, forked from [mpaland/printf](https://github.com/mpaland/printf).  
Plugin made by Linblow.
