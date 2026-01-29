#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 257
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and source preparation
    const char src_c[] = "zlib API sequence payload: backinflate + gzseek64 + gzgetc_ + crc32_z";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();

    // step 2: Initialize inflate-back context with a working window
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc((size_t)(1 << 15));
    memset(window, 0, (size_t)(1 << 15));
    int rc_inflate_back_init = inflateBackInit_(&istrm, 15, window, version, (int)sizeof(z_stream));

    // step 3: Create a gz file, write the source, seek with gzseek64 and read one byte with gzgetc_
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "w+b");
    int rc_gz_write = gzwrite(gz, (voidpc)source, (unsigned int)sourceLen);
    long long pos_after_seek = (long long)gzseek64(gz, (off64_t)0, 0);
    int ch = gzgetc_(gz);
    unsigned char chbyte = (unsigned char)ch;
    Bytef single_buf[1];
    single_buf[0] = (Bytef)chbyte;
    uLong single_crc = crc32_z(0UL, single_buf, (z_size_t)1);

    // step 4: Validate/cleanup inflate-back and gz resources
    int rc_inflate_back_end = inflateBackEnd(&istrm);
    int rc_gz_close = gzclose(gz);
    free(window);

    (void)sourceLen;
    (void)version;
    (void)rc_inflate_back_init;
    (void)rc_gz_write;
    (void)pos_after_seek;
    (void)ch;
    (void)single_crc;
    (void)rc_inflate_back_end;
    (void)rc_gz_close;

    // API sequence test completed successfully
    return 66;
}