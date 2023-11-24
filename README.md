# WEBSCAN

WEBSCAN is a tool to execute pings to all individual ip addresses in a network.
After scanning all individual ip addresses the program will write down the directions
of all online hosts.

## HOW TO USE

### Creating your on compiled file

In order to use WEBSCAN you can also create your own compiled program.
To compile the code you only need to execute the following command:

```bash
gcc -o webscanner scanner.c -lm
```

### Using the compiled file

To use WEBSCAN you need to execute give permissions to the compiled program.

```bash
chmod +x webscanner
```

Once it can be executed, you will have to do the following command to execute the program:

```bash
./webscanner <dir_ip> <mask>
```

Where *dir_ip* is the direction ip of the network and *mask* is the mask of the network to scan.
The *dir_ip* and *mask* need to have the following format: x.x.x.x where the *x* should be octal numbers no larger than 255.