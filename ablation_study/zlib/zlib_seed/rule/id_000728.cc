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
//<ID> 728
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare buffers, input and dictionary
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 256;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'A', (size_t)INPUT_LEN);
    uLong compBound = compressBound(INPUT_LEN);
    Bytef *compBufDict = (Bytef *)malloc((size_t)compBound);
    memset(compBufDict, 0, (size_t)compBound);
    Bytef *compBufPlain = (Bytef *)malloc((size_t)compBound);
    memset(compBufPlain, 0, (size_t)compBound);
    Bytef *outBuf = (Bytef *)malloc((size_t)(INPUT_LEN * 2));
    memset(outBuf, 0, (size_t)(INPUT_LEN * 2));
    Bytef *dict = (Bytef *)malloc(32);
    memset(dict, 'D', 32);

    // step 2: Initialize deflate stream, set dictionary, compress and cleanup deflate
    z_stream def;
    memset(&def, 0, sizeof(def));
    def.next_in = input;
    def.avail_in = (uInt)INPUT_LEN;
    def.next_out = compBufDict;
    def.avail_out = (uInt)compBound;
    deflateInit_(&def, 6, ver, (int)sizeof(z_stream));
    deflateSetDictionary(&def, dict, (uInt)32);
    deflate(&def, 4);
    uLong compLenDict = def.total_out;
    deflateEnd(&def);

    // step 3: Compress plain data with compress2 and then uncompress it
    uLongf compLenPlain = (uLongf)compBound;
    compress2(compBufPlain, &compLenPlain, input, (uLong)INPUT_LEN, 6);
    uLongf destLen = (uLongf)(INPUT_LEN * 2);
    uncompress(outBuf, &destLen, compBufPlain, (uLong)compLenPlain);

    // step 4: Initialize an inflate stream to demonstrate paired init/cleanup and then free resources
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inflateInit_(&inf, ver, (int)sizeof(z_stream));
    inflateEnd(&inf);
    free(input);
    free(compBufDict);
    free(compBufPlain);
    free(outBuf);
    free(dict);
    (void)ver;
    // API sequence test completed successfully
    return 66;
}