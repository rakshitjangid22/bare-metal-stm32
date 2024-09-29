# Bare Metal STM32

## About the project

Building bare-metal firmware for a Cortex-M4 STM32 microcontroller, using open source tools and libraries such as GCC and libopencm3. The project will cover everything from blinking an LED, to building a bootloader for performing firmware updates over UART/USB, to building a signed firmware security mechanism that allow only authorised code to run on the device.

## Prerequisites

You need to have the following installed and properly setup. Ensure that they are available in your path.

- [GNU ARM Embedded tools](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads)
- [GNU Make](https://www.gnu.org/software/make/)

### When using VSCode

You'll want to install some extensions to make development smoother

- `C/C++`
- `Cortex-Debug`

## Repo setup

```bash
# Clone the repo
git clone git@github.com:rakshitjangid22/bare-metal-stm32.git
cd bare-metal-series

# Initialise the submodules (libopencm3)
git submodule init
git submodule update

# Build libopencm3
cd libopencm3
make
cd ..

# Build the main application firmware
cd app
make
```

## Debuggers

### ST-Link

It's recommended that you install the [ST-Link drivers](https://www.st.com/en/development-tools/stsw-link009.html).

You'll also need to install the [open source ST-Link debugging tools](https://github.com/stlink-org/stlink). The primary application you'll need from that tool-set is *stutil*. Verify that *stutil* is available in your path before attempting to use the VSCode ST-Link debugging tasks.

Once your drivers and debugging tools are installed, you can use the "ST-Link: Debug Application" and "ST-Link: Attach active" VSCode tasks to debug your firmware over ST-Link.
