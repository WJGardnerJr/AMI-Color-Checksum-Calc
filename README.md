# AMI-Calc

This program calculates the checksum of AMI Color BIOSes and other AMI BIOSes of 64 KB size using simple summation and modulus. It can do the same for simple 32 KB VGA BIOSes from the same era. It also calculates the additional two bits necessary to set the checksum equal to zero, if modifications are made that otherwise cause the checksum to become invalid.

This can be particularly useful if trying to edit a BIOS in hexidecimal, so as to unhide specific options.

See here for more information about this process: https://www.vogons.org/viewtopic.php?t=82233

Check out the bin folder for pre-compiled binaries:
 - checksumcalc.exe was compiled with g++ on Windows 11 and works on all 32/64-bit Windows and Linux OSes
 - A 16-bit DOS port of this is planned, but not yet completed.
