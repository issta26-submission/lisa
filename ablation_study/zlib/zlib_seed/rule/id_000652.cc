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
//<ID> 652
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 200;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'X', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong compBound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound * 2); // allow space for two passes
    memset(compBuf, 0, (size_t)(compBound * 2));
    uLongf compLen = (uLongf)(compBound * 2);
    const uLong OUT_LEN = INPUT_LEN * 2;
    Bytef *outBuf = (Bytef *)malloc((size_t)OUT_LEN);
    memset(outBuf, 0, (size_t)OUT_LEN);

    // step 2: Initialize and run deflate to produce compressed data
    z_stream def;
    memset(&def, 0, sizeof(def));
    deflateInit_(&def, 6, ver, (int)sizeof(z_stream));
    def.next_in = input;
    def.avail_in = (uInt)src_len;
    def.next_out = compBuf;
    def.avail_out = (uInt)compBound;
    deflate(&def, 4);
    compLen = (uLongf)def.total_out;

    // step 3: Reuse deflate via deflateResetKeep, run again, then finish deflate
    def.next_in = input;
    def.avail_in = (uInt)src_len;
    def.next_out = compBuf + compLen;
    def.avail_out = (uInt)(compBound); 
    deflateResetKeep(&def);
    deflate(&def, 4);
    compLen = (uLongf)def.total_out;
    deflateEnd(&def);

    // step 4: Inflate, call inflateSync, use gz APIs, and cleanup
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inflateInit_(&inf, ver, (int)sizeof(z_stream));
    inf.next_in = compBuf;
    inf.avail_in = (uInt)compLen;
    inf.next_out = outBuf;
    inf.avail_out = (uInt)OUT_LEN;
    inflate(&inf, 0);
    inflateSync(&inf);
    inflateEnd(&inf);

    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, input, (unsigned int)src_len);
    gzclose(gz);
    gz = gzopen("test_zlib_api_sequence.gz", "rb");
    int c = gzgetc_(gz);
    gzclose(gz);

    free(input);
    free(compBuf);
    free(outBuf);
    (void)ver;
    (void)c;
    // API sequence test completed successfully
    return 66;
}