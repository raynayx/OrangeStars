# Setup Raspberry Pi Pico SDK (on Fedora 35)

# Install Dependencies
- CMake
- GNU Arm Embedded Toolchain (arm-none-eabi toolchain)
- Git

# Install SDK
- Clone SDK
```
	
sudo git clone https://github.com/raspberrypi/pico-sdk.git ~/opt/pico-sdk
```

- Initialize submodules
```
sudo git -C ~/opt/pico-sdk submodule update --init
```

- Set PICO_SDK_PATH environment variable

- Source .bashrc to update environment variables
