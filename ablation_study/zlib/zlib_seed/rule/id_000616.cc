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
//<ID> 616
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char * ver = zlibVersion();
    const uLong IN_LEN = 128;
    Bytef * input = (Bytef *)malloc((size_t)IN_LEN);
    memset(input, 'A', (size_t)IN_LEN);
    uLong bound = compressBound(IN_LEN);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    uLongf compLen = (uLongf)bound;
    memset(compBuf, 0, (size_t)bound);
    const uLong OUT_LEN = 512;
    Bytef * outBuf = (Bytef *)malloc((size_t)OUT_LEN);
    memset(outBuf, 0, (size_t)OUT_LEN);

    // step 2: Compress the input into compBuf
    compress(compBuf, &compLen, input, IN_LEN);

    // step 3: Initialize inflate, adjust window via inflateReset2, set buffers and run inflate/sync
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inflateInit_(&inf, ver, (int)sizeof(z_stream));
    inflateReset2(&inf, 15);
    inf.next_in = compBuf;
    inf.avail_in = (uInt)compLen;
    inf.next_out = outBuf;
    inf.avail_out = (uInt)OUT_LEN;
    inflate(&inf, 0);
    inflateSync(&inf);

    // step 4: Compute combined CRC, write original data to gz and cleanup
    uLong crc1 = crc32(0L, input, (uInt)64);
    uLong crc2 = crc32(0L, input + 64, (uInt)(IN_LEN - 64));
    uLong combined_crc = crc32_combine(crc1, crc2, (off_t)(IN_LEN - 64));
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, input, (unsigned int)IN_LEN);
    gzclose(gz);
    inflateEnd(&inf);
    free(input);
    free(compBuf);
    free(outBuf);
    (void)ver;
    (void)combined_crc;
    // API sequence test completed successfully
    return 66;
}