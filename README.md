![Cute Logo](Resources/Icon128.png)

Cute Code Accessor
==================

Cute Code Accessor is a plugin for Unreal Engine that enables the use of Qt Creator as its IDE.
With a minimal setup it generates project files necessary to build the UE project directly from Qt Creator.

It works by parsing the default project files, depending on the platform, to read the necessary defines and include paths to generate qmake project files.
If configured correctly it generates a fully usable project already setup with some of the most common targets.

Setup
=====

Cute Code Accessor can be installed directly from the Unreal Engine Marketplace, here's its page.
TODO: Add link

If your prefer to use the latest version, and don't want to wait for an updated version on the Marketplace, you can clone it into your project or Unreal Engine Plugins folder.

Requirements
============

* Qt Creator 4.7.0
* Unreal Engine 4.20
* Your platform toolchain

Configuration
=============

To start off you must configure the Qt Creator Kit that will be used for Unreal Engine projects.

The Kit must be configured as such:

    Name: Unreal Engine 4
    Device type: Desktop
    Device: Local PC
    Compiler C: Depends on platform (MSVC on Windows, clang on Linux/Mac OS)
    Compile C++: Same as above
    Debugger: Depends on platform (cdb on Windows, gdb on Linux/Mac OS)

These are the necessary settings the rest can safely be ignored.

Name can be different than `Unreal Engine 4` but in this case you must set the name of the Kit to be used in Engine settings under Plugins > Cute Code Accessor, otherwise the projects' build settings can't be generated correctly.

Troubleshooting
===============

Q: Cute Code Accessor can't find Qt Creator.

A: If a valid Qt Creator executable can't be found you can set it from Unreal Engine settings under Plugins > Cute Code Accessor.

---

Q: How can I debug directly from Qt Creator?

A: TODO

---

Q: I have an issue not listed here.

A: Please open an issue specifing Unreal Engine version, Qt Creator version, OS and architecture in use when the issue presented itself. If possible paste your Output Log too, to find only Cute Code Accessor logs search for LogCuteCode.

---

Q: I have changed Unreal Engine version and now have several issues.

A: I suggest deleting all .pro, .pri and .pro.user files whenever switching Engine version.