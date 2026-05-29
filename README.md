# Horizon-Desktop

Experimental Xorg desktop concept art inspired by a sakura sunset scene.

## Build

```bash
make
```

## Run

```bash
./build/horizon-desktop
```

## Run With startx

If you want Horizon Desktop to start as its own X session, create a `~/.xinitrc` file with:

```bash
exec /Users/noahtveit/Documents/GitHub/Horizon-Desktop/build/horizon-desktop
```

Then launch it with:

```bash
startx
```

The project is intentionally lightweight and uses only Xlib, so it can run on a standard Xorg session on Linux.
