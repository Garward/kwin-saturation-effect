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

- KDE Frameworks 6
- KWin development headers
- CMake 3.20+
- Qt 6

On Arch Linux:
```bash
sudo pacman -S cmake extra-cmake-modules kwin
```

On Fedora:
```bash
sudo dnf install cmake extra-cmake-modules kwin-devel
```

On Ubuntu/Debian (KDE Neon):
```bash
sudo apt install cmake extra-cmake-modules kwin-dev
```

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
