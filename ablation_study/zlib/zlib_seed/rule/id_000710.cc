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
//<ID> 710
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers and dictionary
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 256;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'A', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong compBound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLong compLen = 0;
    uLong OUT_LEN = INPUT_LEN * 2;
    Bytef *outBuf = (Bytef *)malloc((size_t)OUT_LEN);
    memset(outBuf, 0, (size_t)OUT_LEN);
    uInt dictLen = 32;
    Bytef *dict = (Bytef *)malloc((size_t)dictLen);
    memset(dict, 'D', (size_t)dictLen);

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

    // step 3: Initialize inflate, set dictionary, copy and reset the inflate stream, then inflate
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inflateInit_(&inf, ver, (int)sizeof(z_stream));
    inflateSetDictionary(&inf, dict, dictLen);
    inf.next_in = compBuf;
    inf.avail_in = (uInt)compLen;
    inf.next_out = outBuf;
    inf.avail_out = (uInt)OUT_LEN;
    inflate(&inf, 0);
    z_stream inf_copy;
    memset(&inf_copy, 0, sizeof(inf_copy));
    inflateCopy(&inf_copy, &inf);
    inflateReset(&inf_copy);
    inflate(&inf_copy, 0);

    // step 4: Validate & cleanup
    uLong checksum = adler32(0UL, outBuf, (uInt)(inf.total_out + inf_copy.total_out));
    inflateEnd(&inf);
    inflateEnd(&inf_copy);
    free(input);
    free(compBuf);
    free(outBuf);
    free(dict);
    (void)ver;
    (void)checksum;
    // API sequence test completed successfully
    return 66;
}