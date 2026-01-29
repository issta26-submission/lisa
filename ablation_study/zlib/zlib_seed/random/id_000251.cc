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
//<ID> 251
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compute its CRC
    const char src_c[] = "zlib API sequence payload: inflateBackInit_ -> gzfile -> crc32_z";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong crc = crc32_z(0UL, source, (z_size_t)sourceLen);

    // step 2: Initialize inflateBack stream with a sliding window buffer
    z_stream backStrm;
    memset(&backStrm, 0, sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc((size_t)(1 << 15));
    memset(window, 0, (size_t)(1 << 15));
    const char *version = zlibVersion();
    int rc_back_init = inflateBackInit_(&backStrm, 15, window, version, (int)sizeof(z_stream));
    backStrm.adler = crc;

    // step 3: Create a gz file, write the source, seek and read one character
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "w+b");
    int rc_gz_write = gzwrite(gz, (voidpc)source, (unsigned int)sourceLen);
    off64_t pos_after_seek = gzseek64(gz, (off64_t)0, 0);
    int first_char = gzgetc_(gz);

    // step 4: Cleanup inflateBack, close gz file and free allocated resources
    int rc_back_end = inflateBackEnd(&backStrm);
    int rc_gz_close = gzclose(gz);
    free(window);

    (void)crc;
    (void)rc_back_init;
    (void)rc_gz_write;
    (void)pos_after_seek;
    (void)first_char;
    (void)rc_back_end;
    (void)rc_gz_close;
    (void)version;
    (void)sourceLen;

    // API sequence test completed successfully
    return 66;
}