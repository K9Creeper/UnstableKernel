<h1> Enviorment Setup </h1>
<h2>CYGWIN Install / Download</h2>

1. Visit the CYGWIN download page at https://www.cygwin.com/
2. Download and Run the installer.
3. Select "Install from Internet"
4. Continue...
5. Select any mirror website.

<h2>CYGWIN Install Packages</h2>
<h3>GCC-G++</h3>

Search for the ```mingw64-i686-gcc-g++``` package, and select it.

<h3>Make</h3>

Search for the ```make``` package, and select it.

<h3>Bison</h3>

Search for the ```bison``` package, and select it.

<h3>Flex</h3>

Search for the ```flex``` package, and select it.

<h3>Libgmp-Devel</h3>

Search for the ```libgmp-devel``` package, and select it.

<h3>Libmpc-Devel</h3>

Search for the ```libmpc-devel``` package, and select it.

<h3>Libmpfr-Devel</h3>

Search for the ```libmpfr-devel``` package, and select it.

<h3>Texinfo</h3>

Search for the ```texinfo``` package, and select it.

<h3>Libisl-Devel</h3>

Search for the ```libisl-devel``` package, and select it.

<h3> Genisoimage </h3>

Search for the ```genisoimage``` package, and select it.


<h2>GRUB for CYGWIN </h2>

Though techinally not apart of Cygwin, GRUB can be downloaded for Windows at https://ftp.gnu.org/gnu/grub/, ex: ```grub-2.06-for-windows.zip```. Once downloaded, rename the extracted folder to ```grub-for-windows```. Then put the fodler into the ```<PARENT_DIR>/cygwin64/home/<USER>/src/``` path, ex: ```C:/cygwin64/home/nickk/src/```.

<h2> CygWin Terminal Setup in Visual Studio Code </h2>

1. Open a terminal, then next to the + symbol, click the drop down, and select default profile.
2. Then, there should be a 'detected' terminal profile, ex: bash (cygwin); and select bash (cygwin) option.
3. Now, you can go back to select default profile and set it back to it's default if wanted.

<h3> TO ACCESS THE PROJECT DIRECTORY IN CygWin Terminal </h3>

1. Locate the directory on your windows machine.
2. Replace the "drive", ex: "C:/", with ex: "/cygdrive/c/". <b> These examples, are based off the cygwin64 folder, located at ```C:/cygwin64/```. </b>
3. In the terminal, run: ```cd <the_path_you_made>```. Example of ```<the_path_you_made>```: ```/cygdrive/c/Users/nickk/OneDrive/Documents/GitHub/UnstableKernel```.