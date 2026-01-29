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
//<ID> 258
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initial CRC computation
    const char src_c[] = "zlib sequence payload: inflateBackInit_, gz file operations and crc32_z";
    const Bytef *source = (const Bytef *)src_c;
    z_size_t sourceLen = (z_size_t)(sizeof(src_c) - 1);
    uLong crc_initial = 0UL;
    uLong crc_val = crc32_z(crc_initial, source, sourceLen);

    // step 2: Setup inflateBack stream and sliding window
    z_stream back_strm;
    memset(&back_strm, 0, sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    const char *version = zlibVersion();
    int rc_back_init = inflateBackInit_(&back_strm, 15, window, version, (int)sizeof(z_stream));

    // step 3: gzFile operations: write data, seek to start, read first byte using gzgetc_
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "w+b");
    int rc_gz_write = gzwrite(gz, (const void *)source, (unsigned int)sourceLen);
    off64_t newpos = gzseek64(gz, (off64_t)0, 0);
    int first_byte = gzgetc_(gz);
    int rc_gz_close = gzclose(gz);

    // step 4: Validate minimal state and cleanup inflateBack resources
    (void)crc_val;
    (void)rc_back_init;
    (void)rc_gz_write;
    (void)newpos;
    (void)first_byte;
    (void)rc_gz_close;
    int rc_back_end = inflateBackEnd(&back_strm);
    free(window);
    (void)rc_back_end;
    (void)version;
    (void)crc_initial;
    (void)sourceLen;

    // API sequence test completed successfully
    return 66;
}