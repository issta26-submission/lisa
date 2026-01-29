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
//<ID> 714
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare version, input data, dictionary and buffers
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 256;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'A', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong compBound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLong OUT_LEN = INPUT_LEN * 2;
    Bytef *outBuf = (Bytef *)malloc((size_t)OUT_LEN);
    memset(outBuf, 0, (size_t)OUT_LEN);
    Bytef dictionary[] = "ZLIBDICT";
    uInt dictLen = (uInt)(sizeof(dictionary) - 1);

    // step 2: Initialize deflate, set a preset dictionary, compress the input
    z_stream def;
    memset(&def, 0, sizeof(def));
    def.next_in = input;
    def.avail_in = (uInt)src_len;
    def.next_out = compBuf;
    def.avail_out = (uInt)compBound;
    deflateInit_(&def, 6, ver, (int)sizeof(z_stream));
    deflateSetDictionary(&def, dictionary, dictLen);
    deflate(&def, 4);
    uLong compLen = def.total_out;
    deflateEnd(&def);

    // step 3: Initialize inflate, set dictionary, copy and reset the inflate stream, then inflate
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inflateInit_(&inf, ver, (int)sizeof(z_stream));
    inflateSetDictionary(&inf, dictionary, dictLen);
    z_stream inf_copy;
    memset(&inf_copy, 0, sizeof(inf_copy));
    inflateCopy(&inf_copy, &inf);
    inflateReset(&inf_copy);
    inf_copy.next_in = compBuf;
    inf_copy.avail_in = (uInt)compLen;
    inf_copy.next_out = outBuf;
    inf_copy.avail_out = (uInt)OUT_LEN;
    inflate(&inf_copy, 0);
    uLong outLen = inf_copy.total_out;
    uLong checksum = adler32(0UL, outBuf, (uInt)outLen);

    // step 4: Cleanup resources
    inflateEnd(&inf_copy);
    inflateEnd(&inf);
    free(input);
    free(compBuf);
    free(outBuf);
    (void)ver;
    (void)checksum;
    // API sequence test completed successfully
    return 66;
}