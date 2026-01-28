sudo apt install qemu-system-i386

---

make qemu-nox

---
rm.asm
objdump -t _rm | sed '1,/SYMBOL TABLE/d; s/ .* / /; /^$/d' > rm.sym
gcc -fno-pic -static -fno-builtin -fno-strict-aliasing -O2 -Wall -MD -ggdb -m32 -Werror -fno-omit-frame-pointer -fno-stack-protector -fno-pie -no-pie   -c -o sh.o sh.c
sh.c: In function ‘runcmd’:
sh.c:58:1: error: infinite recursion detected [-Werror=infinite-recursion]
  58 | runcmd(struct cmd *cmd)
     | ^~~~~~
     
---
sed -i 's/-m32 -Werror -fno-omit/-m32 -Werror -Wno-infinite-recursion -fno-omit/' Makefile
make clean
make qemu-nox

---

inlined from ‘mpinit’ at mp.c:101:14:
mp.c:83:10: error: array subscript -48806446 is outside array bounds of ‘void[2147483647]’ [-Werror=array-bounds=]
   83 |   if(conf->version != 1 && conf->version != 4)
      |      ~~~~^~~~~~~~~
mp.c:78:34: note: at offset -2147483648 into object ‘*<unknown>.physaddr’ of size [0, 2147483647]
   78 |   if((mp = mpsearch()) == 0 || mp->physaddr == 0)
      |                                ~~^~~~~~~~~~
mp.c:85:28: error: array subscript -48806446 is outside array bounds of ‘void[2147483647]’ [-Werror=array-bounds=]
   85 |   if(sum((uchar*)conf, conf->length) != 0)
      |                        ~~~~^~~~~~~~
mp.c:78:34: note: at offset -2147483648 into object ‘*<unknown>.physaddr’ of size [0, 2147483647]
   78 |   if((mp = mpsearch()) == 0 || mp->physaddr == 0)
      |                                ~~^~~~~~~~~~
mp.c: In function ‘mpinit’:

---

sed -i 's/-Wno-infinite-recursion/-Wno-infinite-recursion -Wno-array-bounds/' Makefile

---
