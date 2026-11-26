#!/bin/bash
set -e

cd "$(dirname "$0")"

BUILD_DIR="build"

# 参数：reset/--reset  重置 CMake 缓存，保留 _deps 源码
#       clean/--clean  彻底删除 build 目录（包括 _deps 源码）
#       help/--help    显示用法
case "${1:-build}" in
    reset|--reset)
         echo "重置 CMake 缓存（保留 _deps 源码）..."
        if [ -d "$BUILD_DIR" ]; then
            find "$BUILD_DIR" -mindepth 1 -maxdepth 1 -not -name "_deps" -exec rm -rf {} +
            find "$BUILD_DIR/_deps" -mindepth 1 -maxdepth 1 -not -name "*-src" -exec rm -rf {} +
            echo "完成。"
        else
            echo "build 目录不存在，无需清理。"
        fi
        exit 0 ;;
    clean|--clean)
        if [ -d "$BUILD_DIR" ]; then
            echo "彻底清理 build 目录..."
            rm -rf "$BUILD_DIR"
            echo "完成。"
        else
            echo "build 目录不存在，无需清理。"
        fi
        exit 0 ;;
    help|--help|-h)
        cat <<EOF
用法: $0 [命令]
  （无参数）  configure + build
  reset      重置 CMake 缓存，保留 _deps 源码
  clean      彻底删除 build 目录
  help       显示本帮助
EOF
        exit 0 ;;
    build|--build|"")
        mkdir -p "$BUILD_DIR"
        cd "$BUILD_DIR"
        if [ ! -f CMakeCache.txt ]; then
            cmake .. \
                -DCMAKE_BUILD_TYPE=Debug \
                -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
        fi
        cmake --build . -j"$(nproc)" ;;
    *)
        echo "未知参数: $1" >&2
        echo "用法: $0 [reset|clean|help]" >&2
        exit 1 ;;
esac