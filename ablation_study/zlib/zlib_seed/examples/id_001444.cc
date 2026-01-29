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
//<ID> 1444
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload for crc/combine/gzputc/version";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *buf = new Bytef[(size_t)srcLen];
    memset(buf, 0, (size_t)srcLen);
    memcpy(buf, payload, (size_t)srcLen);
    const char *version = zlibVersion();

    // step 2: Setup - compute CRCs for two sequential blocks (here using the same data twice)
    uLong crc_first = crc32_z(0UL, buf, (z_size_t)srcLen);
    uLong crc_second = crc32_z(0UL, buf, (z_size_t)srcLen);

    // step 3: Core operations - combine CRCs to represent concatenation, write a byte derived from combined CRC to a gzip file
    uLong combined_crc = crc32_combine(crc_first, crc_second, (off_t)srcLen);
    int byte_to_write = (int)(combined_crc & 0xFF);
    gzFile gzf = gzopen("test_zlib_api_sequence_output.gz", "wb");
    int rc_putc = gzputc(gzf, byte_to_write);

    // step 4: Cleanup
    int rc_close = gzclose(gzf);
    delete [] buf;
    (void)version;
    (void)crc_first;
    (void)crc_second;
    (void)combined_crc;
    (void)byte_to_write;
    (void)rc_putc;
    (void)rc_close;
    // API sequence test completed successfully
    return 66;
}