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
//<ID> 686
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
    memset(input, 'P', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong compBound = compressBound(src_len);
    uLongf compLen = (uLongf)compBound;
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);

    // step 2: Compress the input to produce compressed data
    compress2(compBuf, &compLen, input, src_len, 6);

    // step 3: Initialize inflate, perform inflate, then sync and compute adler checksum
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inf.next_in = compBuf;
    inf.avail_in = (uInt)compLen;
    Bytef *decompBuf = (Bytef *)malloc((size_t)src_len);
    memset(decompBuf, 0, (size_t)src_len);
    inf.next_out = decompBuf;
    inf.avail_out = (uInt)src_len;
    inflateInit_(&inf, ver, (int)sizeof(z_stream));
    inflate(&inf, 0);
    inflateSync(&inf);
    uLong adler_after = adler32_z(0L, decompBuf, (z_size_t)src_len);

    // step 4: Write decompressed data to a gz file, cleanup inflate and free resources
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, decompBuf, (unsigned int)src_len);
    gzclose(gz);
    inflateEnd(&inf);
    free(input);
    free(compBuf);
    free(decompBuf);
    (void)ver;
    (void)adler_after;
    // API sequence test completed successfully
    return 66;
}