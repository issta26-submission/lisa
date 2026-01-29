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
//<ID> 772
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 1024;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'A', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong compBound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    compress(compBuf, &compLen, input, src_len);

    // step 2: Write original data into a gzip file and close it (use gzclose)
    gzFile wgz = gzopen("tmp_zlib_api_sequence.gz", "wb");
    gzwrite(wgz, input, (unsigned int)src_len);
    gzclose(wgz);

    // step 3: Initialize inflate, provide compressed data, run inflate, and compute adler32_z
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inf.zalloc = NULL;
    inf.zfree = NULL;
    inf.opaque = NULL;
    inflateInit_(&inf, ver, (int)sizeof(z_stream));
    inf.next_in = compBuf;
    inf.avail_in = (uInt)compLen;
    uLong OUT_LEN = src_len * 2;
    Bytef *outBuf = (Bytef *)malloc((size_t)OUT_LEN);
    memset(outBuf, 0, (size_t)OUT_LEN);
    inf.next_out = outBuf;
    inf.avail_out = (uInt)OUT_LEN;
    inflate(&inf, 0);
    uLong decompressed_len = inf.total_out;
    uLong adler_out = (uLong)adler32_z(0UL, outBuf, (z_size_t)decompressed_len);

    // step 4: Teardown inflate back-end, end inflate, validate via adler32_z for input, cleanup
    inflateBackEnd(&inf);
    inflateEnd(&inf);
    uLong adler_in = (uLong)adler32_z(0UL, input, (z_size_t)src_len);
    free(input);
    free(compBuf);
    free(outBuf);
    (void)ver;
    (void)adler_in;
    (void)adler_out;
    // API sequence test completed successfully
    return 66;
}