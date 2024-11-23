<h1> Cross Compiler Setup (i636) </h1>

<h2> File Explorer Prerequisites </h2>

1. Open File Explorer.
2. Locate and Open your cygwin64 folder, ex: ```C:/cygwin64/```.
3. Open the ```/home/<YOUR_USER>``` directory.

<h2> CygWin Shell / Terminal Prerequisites </h2>

1. Open the CygWin Shell / Terminal.
2. Make sure you are in the correct path: ```/home/<YOUR_USER>```, by using the ```pwd``` command.
3. In the terminal, <b>(in the specfied order)</b> run: ```export PREFIX="$HOME/opt/cross"```, ```export TARGET=i686-elf```, ```export PATH="$PREFIX/bin:$PATH"```.
4. In the terminal, run: ```mkdir src```.
5. In the terminal, run: ```cd src```.

<h2> BinUtils </h2>

<h3> Download </h3>

1. First, download ```binutils-2.43.1.tar.gz``` at https://ftp.gnu.org/gnu/binutils/.
2. Extract the file down to the folder
3. In File Explorer move the folder (binutils-2.43.1), to ```/home/<YOUR_USER>/src```.

<h3> Build </h3>

1. In the terminal, run: ```mkdir build-binutils```.
2. In the terminal, run: ```cd build-binutils```.
3. In the terminal, run: ```../binutils-2.43.1/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror```.
4. Wait for the previous command to finish, this will take time.
5. In the terminal, run: ```make -j 4```. <b> You may replace ```4``` with the number of threads less than or equal to what your proccessor can support. </b>
6. Wait for the previous command to finish, this will take time.
7. In the terminal, run: ```make install -j 4```. <b> You may replace ```4``` with the number of threads less than or equal to what your proccessor can support. </b>
8. Wait for the previous command to finish, this will take time.

<h2> GCC </h2>

<h3> Download </h3>
1. First, download ```gcc-14.2.0.tar.gz``` at https://ftp.gnu.org/gnu/gcc/gcc-14.2.0/.
2. Extract the file down to the folder
3. In File Explorer move the folder (gcc-14.2.0), to ```/home/<YOUR_USER>/src```.

<h3> Build </h3>

1. In the terminal, run: ```cd $HOME/src```.
2. In the terminal, run: ```which -- $TARGET-as || echo $TARGET-as is not in the PATH```.
3. In the terminal, run: ```mkdir build-gcc```.
4. In the terminal, run: ```cd build-gcc```.
5. In the terminal, run: ```../gcc-14.2.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers```.
6. Wait for the previous command to finish, this will take time.
7. In the terminal, run: ```make all-gcc -j 4```. <b> You may replace ```4``` with the number of threads less than or equal to what your proccessor can support. </b>
8. Wait for the previous command to finish, this will take time.
9. In the terminal, run: ```make all-target-libgcc -j 4```. <b> You may replace ```4``` with the number of threads less than or equal to what your proccessor can support. </b>
10. Wait for the previous command to finish, this will take time.
11. In the terminal, run: ```make install-gcc -j 4```. <b> You may replace ```4``` with the number of threads less than or equal to what your proccessor can support. </b>
12. Wait for the previous command to finish, this will take time.
13. In the terminal, run: ```make install-target-libgcc -j 4```. <b> You may replace ```4``` with the number of threads less than or equal to what your proccessor can support. </b>
14. Wait for the previous command to finish, this will take time.

<h2> Post Build </h2>

In the terminal, run: ```export PATH="$HOME/opt/cross/bin:$PATH"```.

<h2> How To Use </h2>

You can use this Cross-Compiler by invoking something like this: ```$TARGET-gcc --version```.


<br>
<br>

<b>Source(s): </b>

https://wiki.osdev.org/GCC_Cross-Compiler
