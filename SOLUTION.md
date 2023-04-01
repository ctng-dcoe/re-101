### Walkthrough
This binary is written in C++ and has several functions
- Opens a socket and retrieves sys.argv[1] 
- Parses the response for \r\n\r\n and returns bytes
- XOR encodes those bytes by 0x30 to set a flag
- Prompts for user input
- Checks the string against the XORed flag
- If matches, print the win flag

### Docker Setup
`docker pull tag`
`docker run -it tag bash` 
Probably ssh into this guy with -X to run ghidra?

### Steps for the class
Load the binary in Ghidra, analyze, and start to identify interesting functions
- How is the flag set?
- What is the win() function?

### Solve 1
There is no host validation on the ctf.ctcyber.io url. You can modify your own /etc/hosts file to point towards that url in order to cheat 

### Solve 2
Simply XOR the returned bytes by 0x30 since they are set by the setFlag() function and pass that string

### Solve 3
Patch out the lines (IDA) where it makes the check, jump right to win()

### Solve 4
Write a hook to modify the key in memory, then supply the key
