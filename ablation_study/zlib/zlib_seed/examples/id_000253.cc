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
//<ID> 253
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and compute initial CRC
    const char src_c[] = "zlib API sequence payload: inflateBackInit_ -> gzfile operations -> crc32_z";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong crc_initial = crc32_z(0UL, source, (z_size_t)sourceLen);

    // step 2: Initialize inflateBack with a sliding window buffer
    z_stream back_strm;
    memset(&back_strm, 0, sizeof(z_stream));
    const int windowBits = 15;
    const int windowSize = 1 << windowBits;
    unsigned char *window = (unsigned char *)malloc((size_t)windowSize);
    memset(window, 0, (size_t)windowSize);
    const char *version = zlibVersion();
    int rc_back_init = inflateBackInit_(&back_strm, windowBits, window, version, (int)sizeof(z_stream));

    // step 3: Create a gz file, write the source, seek and read a byte via gzgetc_, update CRC
    gzFile gz = gzopen64("test_zlib_api_sequence.gz", "w+b");
    int rc_gz_write = gzwrite(gz, (const void *)source, (unsigned int)sourceLen);
    off64_t pos_after_write = gzseek64(gz, 0, 0);
    int read_ch = gzgetc_(gz);
    unsigned char chbyte = (unsigned char)read_ch;
    uLong crc_updated = crc32_z(crc_initial, &chbyte, (z_size_t)1);

    // step 4: Cleanup inflateBack, close gz file and free resources
    int rc_back_end = inflateBackEnd(&back_strm);
    int rc_gz_close = gzclose(gz);
    free(window);

    (void)crc_initial;
    (void)rc_back_init;
    (void)rc_gz_write;
    (void)pos_after_write;
    (void)read_ch;
    (void)crc_updated;
    (void)rc_back_end;
    (void)rc_gz_close;
    (void)version;
    (void)sourceLen;
    (void)source;

    // API sequence test completed successfully
    return 66;
}