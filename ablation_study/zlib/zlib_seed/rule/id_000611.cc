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
//<ID> 611
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 128;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'X', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong bound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;

    // step 2: Compress the input
    compress(compBuf, &compLen, input, src_len);

    // step 3: Initialize inflate stream and perform resets/sync/inflate
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inflateInit_(&inf, ver, (int)sizeof(z_stream));
    inf.next_in = compBuf;
    inf.avail_in = (uInt)compLen;
    const uLong OUT_LEN = 512;
    Bytef *outBuf = (Bytef *)malloc((size_t)OUT_LEN);
    memset(outBuf, 0, (size_t)OUT_LEN);
    inf.next_out = outBuf;
    inf.avail_out = (uInt)OUT_LEN;
    inflateReset2(&inf, 15);
    inflateSync(&inf);
    inflate(&inf, 0);

    // step 4: Compute CRCs, combine, write original to gzip, cleanup
    uLong part = (uLong)(INPUT_LEN / 2);
    uLong crc1 = crc32(0L, input, (uInt)part);
    uLong crc2 = crc32(0L, input + part, (uInt)(INPUT_LEN - part));
    uLong combined_crc = crc32_combine(crc1, crc2, (off_t)(INPUT_LEN - part));
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