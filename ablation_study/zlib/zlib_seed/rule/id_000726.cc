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
//<ID> 726
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare input buffer and produce compressed data using compress2
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

    // step 2: Initialize a deflate stream, set a dictionary, then clean up the deflate stream
    z_stream def;
    memset(&def, 0, sizeof(def));
    deflateInit_(&def, 6, ver, (int)sizeof(z_stream));
    Bytef *dict = (Bytef *)malloc(32);
    memset(dict, 'D', 32);
    deflateSetDictionary(&def, dict, (uInt)32);
    deflateEnd(&def);

    // step 3: Initialize an inflate stream and uncompress the compressed data into an output buffer
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inflateInit_(&inf, ver, (int)sizeof(z_stream));
    uLong OUT_LEN = INPUT_LEN * 2;
    Bytef *outBuf = (Bytef *)malloc((size_t)OUT_LEN);
    memset(outBuf, 0, (size_t)OUT_LEN);
    uLongf outLen = (uLongf)OUT_LEN;
    uncompress(outBuf, &outLen, compBuf, (uLong)compLen);

    // step 4: Cleanup all resources
    inflateEnd(&inf);
    free(dict);
    free(input);
    free(compBuf);
    free(outBuf);
    (void)ver;
    // API sequence test completed successfully
    return 66;
}