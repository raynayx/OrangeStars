# Installing the GCC toolchain for ARM on Fedora(35 as tested)
In order to install the GCC toolchain for ARM on Fedora(35),
- Go to [developer.arm.com](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads#) to fetch the latest compiler set which comes as a compressed file.
- Remove any vestiges of previous installs by running:
    ```bash
    $ sudo dnf remove binutils-arm-none-eabi gcc-arm-none-eabi libnewlib-arm-none-eabi
    ```
- If the feedback is ```No packages marked for removal```, that's fine as there are no previous installations on the sysem.
- Then run the following commands:
    ```bash
    $ mkdir ~/opt   #create opt in $HOME
    $ cd ~/opt      #change into opt
    $ tar -xvf *.bz2    #uncompress the file
    $ chmod -R -w gcc-arm-none-eabi-* #
    ```
- Add the path to the ```.bashrc``` file to make the toolchain available system wide:
    ```
    export PATH=$PATH:~/opt/gcc-arm-none-eabi-*/bin/
    ```
- Source the ```.bashrc``` file thus:
```
$ source ~/.bashrc
```
- If ```libncurses.so.5``` library is not present on the Fedora system, run:
```bash
$ sudo dnf install ncurses-compat-libs
```

### Confirm installation
After installing, you can run the following in a new terminal window:
```
$ arm-none-eabi-gcc --version
```
If you get a feedback stating:
```arm-none-eabi-gcc (GNU Arm Embedded Toolchain 10.3-2021.10) 10.3.1 20210824 (release)``` with dates that match the version you downloaded, then you have correctly installed the ARM Toolchain.