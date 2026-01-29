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
//<ID> 610
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 256;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'A', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong bound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    Bytef *outBuf = (Bytef *)malloc((size_t)src_len);
    memset(outBuf, 0, (size_t)src_len);

    // step 2: Compress the input into compBuf
    compress(compBuf, &compLen, input, src_len);

    // step 3: Initialize inflate stream, feed compressed data and perform operations
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inflateInit_(&inf, ver, (int)sizeof(z_stream));
    inf.next_in = compBuf;
    inf.avail_in = (uInt)compLen;
    inf.next_out = outBuf;
    inf.avail_out = (uInt)src_len;
    inflate(&inf, 0);
    inflateSync(&inf);
    inflateReset2(&inf, 15);

    // step 4: Compute CRCs, write original data to a gzip file, validate and cleanup
    uLong crc_part1 = crc32(0L, input, (uInt)128);
    uLong crc_part2 = crc32(0L, input + 128, (uInt)(INPUT_LEN - 128));
    uLong combined_crc = crc32_combine(crc_part1, crc_part2, (off_t)(INPUT_LEN - 128));
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, input, (unsigned int)INPUT_LEN);
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