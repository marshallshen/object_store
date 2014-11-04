# Object Store in C

## Run tests

1. Deploy code to a CLIC VM
2. If you haven't set up users and groups, you can run `make exec`.
3. Given you have users and groups set up, you can run automated tests
   inside the projects.
   - Tests for `objput` is in `objput_test.sh`, run
     `./objput_test.sh`.
   - Tests for `objget` is in `objget_test.sh`, run
     `./objget_test.sh`.
4. There is also [a video](https://vimeo.com/112025038) that demonstrates the result of the tests.

## IMPORTANT notes
1. The decryption program is a bit FLAKY depending on the random IV
   chosen. If you see the following warning, please rerun the program..

   > [CRYPTO SUPPORT ERROR WARNING]: Decryption error detected..

   Note that the decrypt algorithm will STILL provide incomplete decrypted text.

2. `setuid` verifies if user has read permission before it decrypts the
   file. This is a supported feature from homework 2. If the user does
not have permission to read the file, the program simply returns
"Permission deinied", NO DECRPYTION will happen.

3. All encryption keys are saved in `keys` directory. In theory the keys
   SHOULD BE HIDDEN from all regular users and ONLY ACCESSIBLE by root.
However, due to time constraints, such feature is not supported.

4. Some code is referenced from [Code samples from EVP Symmetric
   Encryption and
Decryption](http://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption)

5. The implementation uses the following:
  - 128-bit MD5 key from passphrase,
  - AES with 128-bit keys and CBC mode
  - 16-byte initialization vector

