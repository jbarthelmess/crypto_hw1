# crypto_hw1
Cryptography and Network Security HW1

I implemented the Toy DES using C. To run this code successfully it requires two processes, one as the 
sender the other as the reciever. They can be compiled as follows
gcc cli_toy_des.c -o sender.exe
gcc serv_toy_des.c -o recieve.exe

To run the code, the reciever must be run first and listening on a port. The command line arguments are:
./recieve.exe <KEY> <FILENAME> <PORT>

The <FILENAME> is the name of the file that the reciever should output the decrypted bytes that it recieves. 
It should be noted that resulting file will have read, write, and execute permissions granted to the user. The 
<KEY> is the encryption key that the reciever should use to decrypt the file contents. The reciever will block 
on an accept call for a TCP connection over which it will recieve encrypted bytes.

Once the reciever is running, run the sender like this:
./sender.exe <KEY> <FILE_PATH> <IP_ADDRESS> <PORT>

Note, the <PORT> and <KEY> arguments should be the same as the ones used to run the reciever. The <IP_ADDRESS> 
is the IP address of the reciever machine. If you are running both sender and reciever locally than <IP_ADDRESS>
should be 127.0.0.1 on Linux machines.  Sender will read the entirety of the file at the <FILE_PATH> provided,
encrypt all bits that it can read, and send them to the reciever in 512 byte chunks. After finishing reading,
encrypting, and sending the file, the sender will terminate. While the network portion of this implementation is
extremely rudimentary, it gets the job done.


The meat of the assignment can be found in the HW1.h file.  The decryption and encryption functions can be found
there.  Each of these functions encrypts (or decrypts) one 8-bit block at a time.  The functions to encrypt (or 
decrypt) full sets of bytes just call these functions over and over again.

For the permutation portions, I wrote a permute function which uses a bit mask to grab each individual bit and add 
them onto a result as dictated by an order array. The order array holds the permutation order given in the assignment
specifications for each of the different permutations given. Several of the functions in HW1.h just set an order 
and call the permute function.  One interesting note is that the orders given in the documentation were given such
that the leftmost bit is the least significant bit, so they appear backward in my implementation to accomodate the 
way the permutation function creates numbers.  

The F boxes have their own function (called f) which initialize the S-boxes, expands the block half, and obtains
the two sets of two bit sections from the S-boxes given from the xor'd result of the expanded block half and 
the given key. The S-boxes are initialized on the heap everytime the f function is called, which is relatively 
inefficient, but its a relatively small thing.

The encryption and decryption functions are almost identical, only changing the order of which key is used first
and last.  They permute the plain text using the permute function, split that permutation in half, and use those 
halves to find the encrypted text by xoring the appropriate halves with calls to the f function.
