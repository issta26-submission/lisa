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
//<ID> 684
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
    uLong compBound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    compress2(compBuf, &compLen, input, src_len, 6);

    // step 2: Initialize inflate and decompress
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inflateInit_(&inf, ver, (int)sizeof(z_stream));
    inf.next_in = compBuf;
    inf.avail_in = (uInt)compLen;
    const uLong OUT_SIZE = src_len * 2;
    Bytef *outBuf = (Bytef *)malloc((size_t)OUT_SIZE);
    memset(outBuf, 0, (size_t)OUT_SIZE);
    inf.next_out = outBuf;
    inf.avail_out = (uInt)OUT_SIZE;
    inflate(&inf, 0);
    inflateSync(&inf);
    uLong decomp_len = inf.total_out;

    // step 3: Compute adler32_z and write decompressed data to gz
    uLong checksum = adler32_z(0L, outBuf, (z_size_t)decomp_len);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, outBuf, (unsigned int)decomp_len);
    gzclose(gz);

    // step 4: Cleanup
    inflateEnd(&inf);
    free(input);
    free(compBuf);
    free(outBuf);
    (void)ver;
    (void)checksum;
    (void)src_len;
    // API sequence test completed successfully
    return 66;
}