#!/bin/bash

echo -ne "acl\t\t";         getfacl --version | head -n1
echo -ne "attr\t\t";        getfattr --version | head -n1
echo -ne "autoconf\t";      autoconf --version | head -n1
echo -ne "automake\t";      automake --version | head -n1
echo -ne "bash\t\t";        bash --version | head -n1
echo -ne "bc\t\t";          bc --version | head -n1
echo -ne "binutils\t";      size --version | head -n1
echo -ne "bison\t\t";       bison --version | head -n1
echo -ne "bzip2\t\t";       bzip2 --version 2>&1 < /dev/null | head -n1
echo -ne "check\t\t";       pkg-config --modversion check
echo -ne "coreutils\t";     uname --version | head -n1
echo -ne "dejagnu\t\t";     runtest --version 2> /dev/null | grep 'DejaGnu'
echo -ne "diffutils\t";     diff --version | head -n1
echo -ne "eudev\t\t";       echo "functionality handled by systemd (udevadm)"
echo -ne "e2fsprogs\t";     pkg-config --modversion ext2fs
echo -ne "expat\t\t";       pkg-config --modversion expat
echo -ne "expect\t\t";      expect -v | head -n1
echo -ne "file\t\t";        file --version | head -n1
echo -ne "findutils\t";     find --version | head -n1
echo -ne "flex\t\t";        flex --version | head -n1
echo -ne "gawk\t\t";        gawk --version | head -n1
echo -ne "gcc\t\t";         gcc --version | head -n1
echo -ne "gdbm\t\t";        gdbmtool --version | head -n1
echo -ne "gettext\t\t";     gettext --version | head -n1
echo -ne "glibc\t\t";       ldd --version | head -n1
echo -ne "gmp\t\t";         echo "import ctypes;var_name='__gmp_version';L=ctypes.cdll.LoadLibrary('/usr/lib/libgmp.so');v=ctypes.c_char_p.in_dll(L,var_name);print(v.value)" | python
echo -ne "gperf\t\t";       gperf --version | head -n1
echo -ne "grep\t\t";        grep --version | head -n1
echo -ne "groff\t\t";       groff --version | head -n1
echo -ne "grub\t\t";        grub-install --version | head -n1
echo -ne "gzip\t\t";        gzip --version | head -n1
echo -ne "iana-etc\t";      cat /etc/protocols | head -n4 | tail -n1
echo -ne "inetutils\t";     ftp --version | head -n1
echo -ne "intltool\t";      intltoolize --version | head -n1
echo -ne "iproute2\t";      ip -V | head -n1
echo -ne "kbd\t\t";         kbdinfo --version | head -n1
echo -ne "kmod\t\t";        kmod --version | head -n1
echo -ne "less\t\t";        less --version | head -n1
echo -ne "libcap\t\t";      ls /usr/lib/libcap.so.*.* | head -n1
echo -ne "libpipeline\t";   ls /usr/lib/libpipeline.so.*.* | head -n1
echo -ne "libtool\t\t";     libtool --version | head -n1
echo -ne "m4\t\t";          m4 --version | head -n1
echo -ne "make\t\t";        make --version | head -n1
echo -ne "mandb\t\t";       mandb --version | head -n1
echo -ne "man-pages\t";     echo "man-pages 5.11"
echo -ne "mpc\t\t";         echo "import ctypes;M=ctypes.cdll.LoadLibrary('/usr/lib/libmpc.so');M.mpc_get_version.restype=ctypes.c_char_p;print(M.mpc_get_version())" | python
echo -ne "mpfr\t\t";        echo "import ctypes;M=ctypes.cdll.LoadLibrary('/usr/lib/libmpfr.so');M.mpfr_get_version.restype=ctypes.c_char_p;print(M.mpfr_get_version())" | python
echo -ne "ncurses\t\t";     clear -V | head -n1
echo -ne "patch\t\t";       patch --version | head -n1
echo -ne "perl\t\t";        echo Perl `perl -V:version`
echo -ne "pkg-config\t";    pkg-config --version | head -n1
echo -ne "procps\t\t";      ps --version | head -n1
echo -ne "psmisc\t\t";      fuser --version 2>&1 < /dev/null | head -n1
echo -ne "readline\t";      ls /usr/lib/libreadline.so.*.* | head -n1
echo -ne "sed\t\t";         sed --version 2>&1 < /dev/null | head -n1
echo -ne "shadow\t\t";      echo "shadow 4.8.1"
echo -ne "sysklogd\t";      echo "functionality handled by systemd (journalctl)"
echo -ne "sysvinit\t";      echo "functionality handled by systemd (init)"
echo -ne "tar\t\t";         tar --version | head -n1
echo -ne "tcl\t\t";         echo 'puts [info patchlevel];exit 0' | tclsh
echo -ne "texinfo\t\t";     texindex --version | head -n1
echo -ne "tzdata\t\t";      echo "tzdata 2021a"
echo -ne "udev-lfs\t";      echo "not applicable with systemd"
echo -ne "util-linux\t";    x86_64 --version | head -n1
echo -ne "vim\t\t";         vim --version | head -n1
echo -ne "xml::parser\t";   echo XML::Parser `perl -MXML::Parser -e 'print "$XML::Parser::VERSION"' 2>/dev/null || echo "Not installed"`
echo -ne "xz utils\t";      xz --version | head -n1
echo -ne "zlib\t\t";        ls /usr/lib/libz.so.*.* | head -n1
echo
echo "Additional packages"
echo -ne "git\t\t";         git --version | head -n1
echo -ne "sudo\t\t";        sudo --version | head -n1
echo -ne "wget\t\t";        wget --version | head -n1
echo -ne "curl\t\t";        curl --version | head -n1
echo
echo -ne "linux-kernel\t";  uname -a
