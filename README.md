BOARD INFO:
------
	Pinout:
		A0:input1
		A1:input2
		A2:TX
		A3:RX
		A4..A7:ADCIN
		A8..A10:RGB led outputs
		A11:Motor output

		B0-B15: output pins

		C0: motherboard control pin

STARTUP:
------
	At power on mcu waits for 2 seconds
	sets C0 to 0
	waits 200 ms
	sets C0 to 1
	waits 200 ms
	sets C0 to 0
	
	after this sequence mcu expect to receive 0x(cafebabedeadbeef) if this message received then mcu sends 0x(deadbeefcafebabe) back
		otherwise power on sequence is repeated
	

MESSAGE PROTOCOL:
------
	Set output pins:
		Send: 		0x01 <2 byte pin states> CRC
		Expect: 	0x55 0x01 <2 byte pin states> CRC
	
	Get output pins:
		Send: 		0x02 CRC
		Expect: 	0x55 0x02 <2 byte pin states> CRC
	
	Set RGB:
		R,G,B values are 1 byte each.
		Send:		0x04 R G B CRC
		Expect:		0x55 0x4 R G B CRC
	
	Get temperature:
		Send:		0x05 CRC
		Expect:		Not implemented
		
	Set motor threshold value:
		Send:		0x6 <1 byte, new value> CRC
		Expect:		0x55 0x6 <new value> CRC

	!!NOT TESTED!!
	If input event occurs 0x1 <1 byte, pin no> CRC is sent by mcu. 
		

SAMPLE MESSAGES:
------
	all messages in hex format
	
	Send 		0202
	expect 		05 <pin states> CRC
	
	clear all pins
	Send 		01000001
	expect		5501000054
	
	set all pins
	Send 		01FFFF01
	expect		5501FFFF54
	
	

USAGE:
------

  install the dependencies (see: INSTALL)

  in the project directory, open a terminal:

      make start-gdb-server


  in the project directory, on another terminal:

      make # compile project

  then use any of them, optional:

      make write              # write binary to MCU directly. No debugging.
      make debug-cmd          # debug using arm-none-eabi-gdb in the command line
      make debug-codeblocks   # debug using code::blocks ide
      make debug-kdbg         # debug using kdbg


INSTALL:
--------

### General Instructions

  Whether Windows, Linux or any other OS, you need the following piece of
  software installed:

      1. Debugger server:
          an application that will forward requests to/from STM32 and
          a local TCP port with GDB protocol

              eg. st-util, openocd

      2. Debugger executable: specifically `arm-none-eabi-gdb`
      3. Compiler: specifically `arm-none-eabi-gcc`
      4. ChibiOS source code
      5. Optionally: A GUI debugger with remote debug capabilities.

            eg. code::blocks, qtcreator, eclipse

          Configure your GUI debugger/IDE:

              1. Debugger SHOULD use project's Makefile when compiling before debug
              2. Debugger MUST use `arm-none-eabi-gdb` binary to connect to the debugger
              3. Debugger MUST connect to remote debugger at "localhost:4242"
                 when debugging
              4. Debugger MUST run the commands (present in `./gdb-init` file)
                 after connecting to the remote debugger
              5. Debugger MUST NOT run the compiled code in the current machine


      You should include all binaries in your PATH environment variable. ChibiOS
      and any other libraries should be in your search path as well.

### Linux instructions:

  1: Debugger server:

        download `st-util` and add the directory to the path:

            mkdir ~/bin
            cd ~/bin
            git clone https://github.com/texane/stlink
            cd stlink
            ./autogen.sh
            ./configure
            make
            sudo make install
            sudo cp *.rules /etc/udev/rules.d/
            # unplug STM32 usb cable
            # plug STM32 usb cable  

  2. and 3.:  Debugger executable and the compiler:

      download and install `gdb-arm-none-eabi`. use one of the following:


          do the following on Debian/Ubuntu:

            sudo apt-get install gdb-arm-none-eabi

          or:

            download the tarball: https://launchpad.net/gcc-arm-embedded/+download
            unzip
            add this path to the `$PATH` variable in `~/.profile` file:

                PATH="$PATH:/path/to/arm-none-eabi-gdb"
                export PATH

            make sure your `.profile` file is sourced. For example, in Debian LXDE,
            we need to do the following:

                cd ~; ln -s .profile .xsessionrc


    4. Download ChibiOS source code:

          download Chibios 3.x:

              cd ~
              git clone https://github.com/ChibiOS/ChibiOS

              # if you dowloaded ChibiOS anywhere else from "$HOME" dir,
              ln -s /path/to/Chibios-3.x ~/ChibiOS

    5. Dowload and configure GUI Debugger: Code::Blocks


          configuration steps for code::blocks:

              1. Project -> Properties // Project Settings --> do tick: "This is a custom Makefile"
              3. Project -> Properties // Debugger

                  Select Target: <Project>

                  Remote Connection:
                      IP address: localhost
                      Port: 4242

                  Additional GDB Commands:
                      After Connection:
               4.        (copy and paste `./gdb-init` file contents here)

               2. Settings -> Compiler and Debugger Settings //
                  // Global Compiler Settings
                    // Selected compiler: select any name you want
                    // Debugger executable path: $(which arm-none-eabi-gdb)
                5.  // Do *not* run the debugee

### Windows

    1.: Install "st-util" gdb server:

          Search net for: "how to install st-util gdb server for windows"
          Follow the instructions

    2. and 3.: Download and install "gcc-arm-embedded toolchain"

    4. Download ChibiOS source code as described in "Linux" section.

    5. Download your favourite IDE and configure it as described in either "Linux"
       section or "General instructions" section.

       
