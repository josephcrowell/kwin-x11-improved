# Contributing to KWin

## Chatting

Come on by and ask about anything you run into when hacking on KWin!

KWin's Matrix room on our instance is located here: https://matrix.to/#/#kwin:kde.org.
You can grab an Matrix account at https://webchat.kde.org/ if you don't already have one from us or another provider.

The Matrix room is bridged to `#kde-kwin` on Libera, allowing IRC users to access it.

## What Needs Doing

There's a large amount of bugs open for KWin on our [Bugzilla instance](https://bugs.kde.org/describecomponents.cgi?product=kwin).

## Where Stuff Is

Everything codewise for KWin itself is located in the `src` directory.

### Settings Pages / KCMs

All the settings pages for KWin found in System Settings are located in `src/kcmkwin`.

### Default Decorations

The Breeze decorations theme is not located in the KWin repository, and is in fact part of the [Breeze repository here](https://invent.kde.org/plasma/breeze), in `kdecoration`.

### Tab Switcher

The default visual appearance of the tab switcher is located in `src/tabbox/switchers`.

Other window switchers usually shipped by default are located in [Plasma Addons](https://invent.kde.org/plasma/kdeplasma-addons), located in the `kwin/windowswitchers` directory.

### Window Management

Most window management stuff (layouting, movement, properties, communication between client<->server) is defined in files ending with `client`, such as `x11client.cpp` and `xdgshellclient.cpp`.

### Window Effects

Window effects are located in `src/plugins`, one effect plugin per folder.  Folder `src/plugins/private` contains the plugin (`org.kde.kwin_x11.private.effects`) that exposes layouting properties and `WindowHeap.qml` for QML effects.  Not everything here is an effect as exposed in the configuration UI, such as the colour picker in `src/plugins/colorpicker`.

Of note, the Effects QML engine is shared with the Scripting components (see `src/scripting`).

### Scripting API

Many objects in KWin are exposed directly to the scripting API; scriptable properties are marked with Q_PROPERTY and functions that scripts can invoke on them.

Other scripting stuff is located in `src/scripting`.

## Conventions

### Coding Conventions

KWin's coding conventions are located [here](doc/coding-conventions.md).

KWin additionally follows [KDE's Frameworks Coding Style](https://community.kde.org/Policies/Frameworks_Coding_Style).

### Commits

We usually use this convention for commits in KWin:

```
component/subcomponent: Do a thing

This is a body of the commit message,
elaborating on why we're doing thing.
```

While this isn't a hard rule, it's appreciated for easy scanning of commits by their messages.

## Contributing

KWin uses KDE's GitLab instance for submitting code.

You can read about the [KDE workflow here](https://community.kde.org/Infrastructure/GitLab).

## Running KWin From Source

KWin uses CMake like most KDE projects, so you can build it like this:

```bash
mkdir _build
cd _build
cmake ..
make
```

People hacking on much KDE software may want to set up [kdesrc-build](https://invent.kde.org/sdk/kdesrc-build).

Once built, you can either install it over your system KWin (not recommended) or run it from the build directory directly.

Running it from your build directory looks like this:
```bash
# from the root of your build directory

source prefix.sh
cd bin

# for wayland, starts nested session: with console

env QT_PLUGIN_PATH="$(pwd)":"$QT_PLUGIN_PATH" dbus-run-session ./kwin_wayland --xwayland konsole

# or for x11, replaces current kwin instance:

env QT_PLUGIN_PATH="$(pwd)":"$QT_PLUGIN_PATH" ./kwin_x11 --replace

```

QT_PLUGIN_PATH tells Qt to load KWin's plugins from the build directory, and not from your system KWin.

The dbus-run-session is needed to prevent the nested KWin instance from conflicting with your session KWin instance when exporting objects onto the bus, or with stuff like global shortcuts.

If you need to run a whole Wayland plasma session, you should install a development session by first building [plasma-workspace](https://invent.kde.org/plasma/plasma-workspace) and executing the `login-sessions/install-sessions.sh` in the build directory. This can be done using kdesrc-build.

```bash
kdesrc-build plasma-workspace
# assuming the root directory for kdesrc-build is ~/kde
bash ~/kde/build/plasma-workspace/login-sessions/install-sessions.sh
```
Then you can select the develop session in the sddm login screen.

You can look up the current boot kwin log via `journalctl --user-unit plasma-kwin_wayland --boot 0`.

## Using A Debugger

Trying to attach a debugger to a running KWin instance from within itself will likely be the last thing you do in the session, as KWin will freeze until you resume it from your debugger, which you need KWin to interact with.

Instead, either attach a debugger to a nested KWin instance or debug over SSH.

## Tests

KWin has a series of unit tests and integration tests that ensure everything is running as expected.

If you're adding substantial new code, it's expected that you'll write tests for it to ensure that it's working as expected.

If you're fixing a bug, it's appreciated, but not expected, that you add a test case for the bug you fix.

You can read more about [KWin's testing infrastructure here](doc/TESTING.md).
