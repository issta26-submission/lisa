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
//<ID> 259
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compute CRC
    const char src_c[] = "zlib API sequence payload: inflateBackInit_ -> gzfile roundtrip -> crc32_z";
    const Bytef *source = (const Bytef *)src_c;
    z_size_t src_zlen = (z_size_t)(sizeof(src_c) - 1);
    uLong sourceLen = (uLong)src_zlen;
    uLong crc = crc32_z(0UL, source, src_zlen);

    // step 2: Initialize inflate-back stream with a window buffer
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc((size_t)32768);
    memset(window, 0, (size_t)32768);
    const char *version = zlibVersion();
    int rc_inflateback_init = inflateBackInit_(&istrm, 15, window, version, (int)sizeof(z_stream));
    istrm.adler = crc; // propagate computed CRC into stream state for validation-like flow

    // step 3: Write source to a gz file, seek, and read a single character
    gzFile gz = gzopen64("test_zlib_api_sequence.gz", "w+b");
    unsigned int wrote = (unsigned int)gzwrite(gz, (voidpc)source, (unsigned int)sourceLen);
    off64_t seek_pos = gzseek64(gz, (off64_t)0, 0);
    int first_char = gzgetc_(gz);

    // step 4: Cleanup inflate-back and gz resources
    int rc_inflateback_end = inflateBackEnd(&istrm);
    int rc_gz_close = gzclose(gz);
    free(window);

    (void)rc_inflateback_init;
    (void)rc_inflateback_end;
    (void)rc_gz_close;
    (void)wrote;
    (void)seek_pos;
    (void)first_char;
    (void)crc;
    (void)version;
    (void)sourceLen;

    // API sequence test completed successfully
    return 66;
}