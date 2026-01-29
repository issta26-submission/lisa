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
//<ID> 679
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
    uLong compLen = 0;
    Bytef *decompBuf = (Bytef *)malloc((size_t)(INPUT_LEN * 2));
    memset(decompBuf, 0, (size_t)(INPUT_LEN * 2));
    uLongf decompLen = (uLongf)(INPUT_LEN * 2);

    // step 2: Initialize, configure and run deflate to produce compressed data
    z_stream def;
    memset(&def, 0, sizeof(def));
    def.next_in = input;
    def.avail_in = (uInt)src_len;
    def.next_out = compBuf;
    def.avail_out = (uInt)compBound;
    deflateInit_(&def, 6, ver, (int)sizeof(z_stream));
    deflate(&def, 4);
    compLen = def.total_out;
    deflateEnd(&def);

    // step 3: Use uncompress2 to decompress compressed data into a buffer
    uncompress2(decompBuf, &decompLen, compBuf, &compLen);

    // step 4: Initialize inflate, exercise inflateBackEnd, and use gzputc to write a byte
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inflateInit_(&inf, ver, (int)sizeof(z_stream));
    inf.next_in = compBuf;
    inf.avail_in = (uInt)compLen;
    Bytef *inflateOut = (Bytef *)malloc((size_t)(INPUT_LEN * 2));
    memset(inflateOut, 0, (size_t)(INPUT_LEN * 2));
    inf.next_out = inflateOut;
    inf.avail_out = (uInt)(INPUT_LEN * 2);
    inflate(&inf, 0);
    inflateBackEnd(&inf);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputc(gz, (int)inflateOut[0]);
    gzclose(gz);
    inflateEnd(&inf);

    free(input);
    free(compBuf);
    free(decompBuf);
    free(inflateOut);
    (void)ver;
    (void)decompLen;
    (void)src_len;
    (void)compBound;
    // API sequence test completed successfully
    return 66;
}