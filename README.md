# Godot Steam
Steam API for the Godot game engine. For the Windows, Linux, and Mac platforms. Mac is currently experimental.

How to use
----------
- Download [Steamworks SDK](https://partner.steamgames.com); this requires a Steam developer account.
- Download [Godot source](https://github.com/godotengine/godot); preferably 2.0.3 or higher. 
- Copy SDK files:

  ```
  sdk/public
  sdk/redistributable_bin
  ```
  To
  ```
  godotsteam/sdk
  ```
  The directory contents should look like this:
  ```
  godotsteam/sdk/public/*
  godotsteam/sdk/redistributable_bin/*
  godotsteam/SCsub
  godotsteam/config.py
  godotsteam/godotsteam.cpp
  godotsteam/godotsteam.h
  godotsteam/register_types.cpp
  godotsteam/register_types.h
  ```
- Move the "godotsteam" directory inside the "modules" directory on the Godot source. Recompile for your platform.

  For Linux, if not using Godot 2.0.3 or higher, you must add ```openssl=no``` when compile because it has problem with libcrypto (class StreamPeerSSL can't use).
  
  For Windows it's highly advised to use Visual Studio. With some extra steps MinGW will work, but most likely with limited functionality.

- Copy shared library (steam_api) from /sdk/redistributable_bin/ folders to Godot binary location, should look like this (depending on target OS):

  Linux 32/64-Bit
  ```
  ./libsteam_api.so
  ./godot_binary
  ```
  Mac 32-Bit
  ```
  ./libsteam_api.dylib
  ./godot_binary
  ```
  Windows 32-Bit
  ```
  ./steam_api.dll
  ./godot_binary.exe
  ```
  Windows 64-Bit
  ```
  ./steam_api64.dll
  ./godot_binary.exe
  ```
- Game must ship with exe, Steam API DLL, SO, or DyLIB, and steam_appid.txt to function. Lack of Steam API DLL, SO, DyLIB (for respective OS), or steam_appid.txt will cause it to fail.

Steamworks API Reference
-------------
![alt tag](https://raw.githubusercontent.com/Gramps/GodotSteam/master/godotsteam.png)

To-Do
-------------
- Mac version needs additional testing (I do not own a Mac).
- Add additional functionality, ideally all of the Steamworks API.
- Rewrite the examples section to fit this new layout.

License
-------------
MIT license
