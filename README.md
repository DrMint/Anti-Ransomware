# RansomTraps

Welcome! This is the first time this program is been launched.
RansomTraps is a free anti-ransomware program. It is variant resistant, it doesn't rely on the ransomware's signature to detect an attack. 

Here's how this works: The program generates random 'dummy files' in locations specified by the 'initfile' inside the program folder. A dummy file is a file that is only used by that program; it contains a random string. The program check on that file from time to time to make sure nothing has been encrypted. If a dummy file has been altered or deleted, the program will alert the user and quickly shutdown the computer. It is highly recommended to NOT restart the computer after this, but instead to properly copy your data to another disk if you don't already have a backup system. 

Dummy files will have random names and their extension will be DOCX, JPG, MP3 and ODT.
