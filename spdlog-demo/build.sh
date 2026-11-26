#!/bin/bash
set -e

cd "$(dirname "$0")"

BUILD_DIR="build"

case "${1:-}" in
    reset|--reset)
        echo "重置 CMake 缓存（保留 _deps 源码）..."
        if [ -d "$BUILD_DIR" ]; then
            find "$BUILD_DIR" -mindepth 1 -maxdepth 1 -not -name "_deps" -exec rm -rf {} +
            find "$BUILD_DIR/_deps" -mindepth 1 -maxdepth 1 -not -name "*-src" -exec rm -rf {} + 2>/dev/null || true
            echo "完成。"
        else
            echo "$BUILD_DIR 目录不存在，无需清理。"
        fi
        exit 0 ;;
    clean|--clean)
        if [ -d "$BUILD_DIR" ]; then
            echo "彻底清理 $BUILD_DIR 目录..."
            rm -rf "$BUILD_DIR"
            echo "完成。"
        else
            echo "$BUILD_DIR 目录不存在，无需清理。"
        fi
        exit 0 ;;
    help|--help|-h)
        cat <<EOF
用法: $0 <release|debug|reset|clean|help>

  release   Release 构建（-O3 -DNDEBUG）
  debug     Debug 构建（-g -O0）
  reset     重置 CMake 缓存，保留 _deps 源码
  clean     彻底删除 build 目录
  help      显示本帮助

使用单一 build/ 目录，切换 debug/release 时自动重配。
EOF
        exit 0 ;;
    release|rel)
        CMAKE_TYPE="Release" ;;
    debug|dbg)
        CMAKE_TYPE="Debug" ;;
    "")
        echo "错误: 必须指定 release 或 debug" >&2
        echo "用法: $0 <release|debug|reset|clean|help>" >&2
        exit 1 ;;
    *)
        echo "未知参数: $1" >&2
        echo "用法: $0 <release|debug|reset|clean|help>" >&2
        exit 1 ;;
esac

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# 检查 cache 中已有的构建类型是否匹配，不匹配则清 cache 重配
if [ -f CMakeCache.txt ]; then
    cached_type=$(grep '^CMAKE_BUILD_TYPE:' CMakeCache.txt 2>/dev/null | cut -d= -f2 || true)
    if [ "$cached_type" != "$CMAKE_TYPE" ]; then
        echo "切换构建类型: ${cached_type:-无} -> $CMAKE_TYPE，清理缓存..."
        find . -maxdepth 1 -not -name '_deps' -not -name '.' -exec rm -rf {} +
        find _deps -maxdepth 1 -not -name '*-src' -exec rm -rf {} + 2>/dev/null || true
    fi
fi

if [ ! -f CMakeCache.txt ]; then
    echo "Configure: $CMAKE_TYPE"
    cmake .. \
        -DCMAKE_BUILD_TYPE="$CMAKE_TYPE" \
        -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
fi

echo "Build: $CMAKE_TYPE"
cmake --build . -j"$(nproc)"