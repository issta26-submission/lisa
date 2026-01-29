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
//<ID> 694
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare input data and allocate buffers
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 512UL;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'A', (size_t)INPUT_LEN);
    uLong src_len = INPUT_LEN;
    uLong compBound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);

    // step 2: Initialize deflate, compress data, and finalize deflate
    z_stream def;
    memset(&def, 0, sizeof(def));
    def.next_in = input;
    def.avail_in = (uInt)src_len;
    def.next_out = compBuf;
    def.avail_out = (uInt)compBound;
    deflateInit_(&def, 6, ver, (int)sizeof(z_stream));
    deflate(&def, 4);
    uLong compLen = def.total_out;
    deflateEnd(&def);

    // step 3: Initialize inflate, decompress, inspect codes used, and compute adler32
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inflateInit_(&inf, ver, (int)sizeof(z_stream));
    inf.next_in = compBuf;
    inf.avail_in = (uInt)compLen;
    uLong OUT_LEN = INPUT_LEN * 2UL;
    Bytef *inflateOut = (Bytef *)malloc((size_t)OUT_LEN);
    memset(inflateOut, 0, (size_t)OUT_LEN);
    inf.next_out = inflateOut;
    inf.avail_out = (uInt)OUT_LEN;
    inflate(&inf, 0);
    unsigned long codes_used = inflateCodesUsed(&inf);
    uLong checksum = adler32(0UL, inflateOut, (uInt)inf.total_out);

    // step 4: Write a byte to a gzip file using gzputc, close and cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputc(gz, (int)inflateOut[0]);
    gzclose(gz);
    inflateEnd(&inf);

    free(input);
    free(compBuf);
    free(inflateOut);
    (void)ver;
    (void)codes_used;
    (void)checksum;
    // API sequence test completed successfully
    return 66;
}