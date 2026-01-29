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
//<ID> 677
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
    uLongf compLen_f = (uLongf)compBound;

    // step 2: Initialize, configure and run deflate to produce compressed data
    z_stream def;
    memset(&def, 0, sizeof(def));
    def.next_in = input;
    def.avail_in = (uInt)src_len;
    def.next_out = compBuf;
    def.avail_out = (uInt)compBound;
    deflateInit_(&def, 6, ver, (int)sizeof(z_stream));
    deflate(&def, 4);
    compLen_f = (uLongf)def.total_out;
    deflateEnd(&def);

    // step 3: Decompress using uncompress2 and exercise inflate initialization/back-end
    uLongf outLen_f = (uLongf)src_len;
    Bytef *outBuf = (Bytef *)malloc((size_t)outLen_f);
    memset(outBuf, 0, (size_t)outLen_f);
    uLong compSrcLen = (uLong)compLen_f;
    uncompress2(outBuf, &outLen_f, compBuf, &compSrcLen);
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inflateInit_(&inf, ver, (int)sizeof(z_stream));
    inflateBackEnd(&inf);
    inflateEnd(&inf);

    // step 4: Use gz APIs, validate and cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputc(gz, 'Z');
    gzclose(gz);
    uLong crc_in = crc32(0L, input, (uInt)src_len);
    uLong crc_out = crc32(0L, outBuf, (uInt)outLen_f);
    free(input);
    free(compBuf);
    free(outBuf);
    (void)ver;
    (void)compLen_f;
    (void)crc_in;
    (void)crc_out;
    // API sequence test completed successfully
    return 66;
}