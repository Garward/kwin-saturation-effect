# Saturation Boost - KWin Effect

A KWin desktop effect that increases color saturation/vibrance for more vivid colors. This is the Wayland/KDE equivalent of NVIDIA Digital Vibrance or AMD Saturation settings.

## Features

- Adjustable saturation level (0% to 400%)
- Works on both Wayland and X11
- Proper sRGB color space handling for accurate color manipulation
- Minimal performance impact
- Configurable via KDE System Settings

## Screenshots

*Coming soon*

## Installation

### From Source

#### Dependencies

Requires KDE Plasma 6 with KWin 6, KDE Frameworks 6, Qt 6, and CMake 3.20+.

- Arch:
  ```bash
  sudo pacman -S --needed git gcc cmake extra-cmake-modules kwin kcmutils ki18n qt6-base
  ```
- Debian / Ubuntu (Plasma 6):
  ```bash
  sudo apt install git g++ cmake extra-cmake-modules kwin-dev libkf6kcmutils-dev libkf6i18n-dev libkf6config-dev qt6-base-dev gettext
  ```
- Fedora (Plasma 6):
  ```bash
  sudo dnf install git gcc-c++ cmake extra-cmake-modules kwin-devel kf6-kcmutils-devel kf6-ki18n-devel kf6-kconfig-devel qt6-qtbase-devel
  ```
- openSUSE Tumbleweed:
  ```bash
  sudo zypper in git gcc-c++ cmake extra-cmake-modules kwin6-devel kf6-kcmutils-devel kf6-ki18n-devel kf6-kconfig-devel qt6-base-devel
  ```

> ECM 6.0+ is required. On older distros where the system-wide `extra-cmake-modules` is still 5.x, install the KDE 6 dev packages above before running `cmake`.

#### Build & Install

```bash
git clone https://github.com/garward/kwin-saturation-effect.git
cd kwin-saturation-effect
mkdir build && cd build
cmake ..
make -j$(nproc)
sudo make install
```

#### Enable the Effect

After installation, either:
- Go to **System Settings > Desktop Effects** and enable "Saturation Boost"
- Or run: `kwriteconfig6 --file kwinrc --group Plugins --key saturationEnabled true && qdbus6 org.kde.KWin /KWin reconfigure`

## Configuration

The saturation level can be configured in `~/.config/kwinrc`:

```ini
[Effect-saturation]
Saturation=2.0
```

Values:
- `1.0` = Normal (100% saturation)
- `1.5` = 150% saturation
- `2.0` = 200% saturation (equivalent to nvibrant 1023)
- `0.0` = Grayscale

After changing the config, reload KWin:
```bash
qdbus6 org.kde.KWin /KWin reconfigure
```

## Troubleshooting

**Effect doesn't appear in System Settings**
Log out and back in — KWin only scans for new effects at session start. Confirm the `.so` landed in the right plugin dir for your distro (typically `/usr/lib/qt6/plugins/kwin/effects/plugins/` on Arch/Debian or `/usr/lib64/qt6/plugins/kwin/effects/plugins/` on Fedora/openSUSE).

**Build installs to `/usr/lib/qt5/...` instead of qt6**
Your system has both Qt5 and Qt6 ECM modules and CMake picked the older one. Make sure the KDE 6 dev packages above are installed; the build pins ECM to 6.0+ to avoid this.

## Why This Effect?

On X11, tools like `vibrant-cli` or `nvibrant` can adjust saturation via the GPU driver's CTM (Color Transformation Matrix). However, on Wayland, KWin holds exclusive access to the DRM master, making direct CTM manipulation impossible.

This KWin effect provides a proper Wayland-native solution by applying the saturation transformation as a post-processing shader effect.

## Technical Details

The effect works by:
1. Capturing each window's rendered output to an offscreen texture
2. Converting from sRGB to linear color space
3. Applying the saturation formula: `output = luminance + S × (input - luminance)`
4. Converting back to sRGB
5. Using BT.709 luminance coefficients for accurate perception-based saturation

## License

GPL-2.0-or-later

## Credits

- Inspired by [vibrant-cli](https://github.com/libvibrant/vibrant-cli) and nvibrant
- Based on KWin's colorblindness correction effect architecture
