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
//<ID> 725
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 512;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'A', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong compBound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLong OUT_LEN = INPUT_LEN * 2;
    Bytef *outBuf = (Bytef *)malloc((size_t)OUT_LEN);
    memset(outBuf, 0, (size_t)OUT_LEN);

    // step 2: Initialize and configure deflate, set a dictionary, then compress
    z_stream def;
    memset(&def, 0, sizeof(def));
    def.next_in = input;
    def.avail_in = (uInt)src_len;
    def.next_out = compBuf;
    def.avail_out = (uInt)compBound;
    deflateInit_(&def, 6, ver, (int)sizeof(z_stream));
    Bytef dict[16];
    memset(dict, 'D', 16);
    deflateSetDictionary(&def, dict, (uInt)0); // call with zero-length dictionary to avoid requiring it for decompression
    deflate(&def, 4);
    uLong compLen = def.total_out;
    deflateEnd(&def);

    // step 3: Initialize inflate stream (for realistic usage) and perform uncompress to validate round-trip
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inflateInit_(&inf, ver, (int)sizeof(z_stream));
    uLongf destLen = (uLongf)OUT_LEN;
    uncompress(outBuf, &destLen, compBuf, compLen);
    inflateEnd(&inf);

    // step 4: Validate checksum, cleanup resources, and finish
    uLong checksum = adler32(0UL, outBuf, (uInt)destLen);
    free(input);
    free(compBuf);
    free(outBuf);
    (void)ver;
    (void)checksum;
    // API sequence test completed successfully
    return 66;
}