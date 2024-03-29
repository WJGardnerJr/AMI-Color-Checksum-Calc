# AMI-Color-Checksum-Calc

This program calculates the checksum of AMI Color BIOSes and other AMI BIOSes of 64 KB size using a simple summation and modulus algorithm. It can do the same for 32 KB VGA BIOSes from the same era. For AMI BIOSes only, also calculates the additional two bits necessary to set the checksum equal to zero if modifications are made that otherwise cause the checksum to become invalid.

This can be particularly useful if trying to edit a BIOS in hexidecimal, so as to unhide specific menu options.

This program can be called in a couple of ways:
 - You can simply launch it and interact with the terminal.
 - Command line arguments are parsed, so you can call the program from a Windows command line as follows:
    - For 64 KB AMI Color (or older style AMI) BIOSes, use: 
        - checksumcalc.exe -ami filename.bin 
        - checksumcalc.exe ami filename.bin
    - For 32 KB VGA BIOSes, use:
        - checksumcalc.exe -vga filename.bin 
        - checksumcalc.exe vga filename.bin
  - Likewise, for a *nix command line, use the form:
    - For 64 KB AMI Color (or older style AMI) BIOSes, use: 
        - ./checksumcalc -ami filename.bin 
        - ./checksumcalc ami filename.bin
    - For 32 KB VGA BIOSes, use:
        - ./checksumcalc -vga filename.bin 
        - ./checksumcalc vga filename.bin

See here for more information about this process: https://www.vogons.org/viewtopic.php?t=82233

Check out the bin folder for pre-compiled binaries:
 - checksumcalc.exe was compiled with g++ on Windows 11 and works on all 32/64-bit Windows OSes
 - checksumcalc was compiled on Debian 11 Bullseye and works on all *nix-based OSes
 - A 16-bit DOS port of this is planned, but not yet completed.
