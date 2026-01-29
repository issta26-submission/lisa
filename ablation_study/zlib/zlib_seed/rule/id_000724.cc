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
//<ID> 724
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare input, dictionary and buffers
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 512;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'A', (size_t)INPUT_LEN);
    Bytef *dictionary = (Bytef *)malloc(64);
    memset(dictionary, 'D', 64);
    uLong src_len = (uLong)INPUT_LEN;
    uLong compBound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf outLen = (uLongf)INPUT_LEN;
    Bytef *outBuf = (Bytef *)malloc((size_t)outLen);
    memset(outBuf, 0, (size_t)outLen);

    // step 2: Initialize deflate, set dictionary, compress input, and cleanup deflate
    z_stream def;
    memset(&def, 0, sizeof(def));
    def.next_in = input;
    def.avail_in = (uInt)src_len;
    def.next_out = compBuf;
    def.avail_out = (uInt)compBound;
    deflateInit_(&def, 6, ver, (int)sizeof(z_stream));
    deflateSetDictionary(&def, dictionary, (uInt)64);
    deflate(&def, 4);
    uLong compLen = def.total_out;
    deflateEnd(&def);

    // step 3: Initialize an inflate stream (for realistic sequence) and use uncompress to decompress
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inflateInit_(&inf, ver, (int)sizeof(z_stream));
    uLongf destLen = outLen;
    uncompress(outBuf, &destLen, compBuf, (uLong)compLen);

    // step 4: Cleanup streams and buffers
    inflateEnd(&inf);
    free(input);
    free(dictionary);
    free(compBuf);
    free(outBuf);

    // API sequence test completed successfully
    return 66;
}