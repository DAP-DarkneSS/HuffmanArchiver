curl -s https://build.opensuse.org/build/home:DarkSS/openSUSE_Factory/x86_64/HuffmanArchiver/_log | grep -vE "mkswap|clang" | grep -E " warning: |scan-build:| error: |\.ha"
