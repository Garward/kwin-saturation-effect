# Maintainer: Garward <your-email@example.com>
pkgname=kwin-effect-saturation-git
pkgver=1.1.0.r0.g0000000
pkgrel=1
pkgdesc="KWin effect that increases color saturation/vibrance for more vivid colors"
arch=('x86_64')
url="https://github.com/Garward/kwin-saturation-effect"
license=('GPL-2.0-or-later')
depends=('kwin')
makedepends=('git' 'cmake' 'extra-cmake-modules' 'kcmutils')
provides=('kwin-effect-saturation')
conflicts=('kwin-effect-saturation')
source=("git+https://github.com/Garward/kwin-saturation-effect.git")
sha256sums=('SKIP')

pkgver() {
    cd kwin-saturation-effect
    git describe --long --tags 2>/dev/null | sed 's/^v//;s/\([^-]*-g\)/r\1/;s/-/./g' ||
    printf "1.1.0.r%s.g%s" "$(git rev-list --count HEAD)" "$(git rev-parse --short HEAD)"
}

build() {
    cmake -B build -S kwin-saturation-effect \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=/usr
    cmake --build build
}

package() {
    DESTDIR="$pkgdir" cmake --install build
}
