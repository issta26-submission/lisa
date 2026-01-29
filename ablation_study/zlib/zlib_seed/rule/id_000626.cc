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
//<ID> 626
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
    uLong bound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    compress2(compBuf, &compLen, input, src_len, 6);
    const uLong OUT_LEN = 512;
    Bytef *outBuf = (Bytef *)malloc((size_t)OUT_LEN);
    memset(outBuf, 0, (size_t)OUT_LEN);

    // step 2: Initialize and configure inflate stream
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inflateInit2_(&inf, 15, ver, (int)sizeof(z_stream));
    inf.next_in = compBuf;
    inf.avail_in = (uInt)compLen;
    inf.next_out = outBuf;
    inf.avail_out = (uInt)OUT_LEN;
    Bytef dictBuf[32];
    memset(dictBuf, 'D', sizeof(dictBuf));
    uInt dictLen = (uInt)sizeof(dictBuf);
    inflateSetDictionary(&inf, dictBuf, dictLen);

    // step 3: Operate (inflate), copy stream state, and reset a deflate stream keep-state
    int inf_ret = inflate(&inf, 0);
    z_stream inf_copy;
    memset(&inf_copy, 0, sizeof(inf_copy));
    inflateCopy(&inf_copy, &inf);
    z_stream def;
    memset(&def, 0, sizeof(def));
    deflateResetKeep(&def);

    // step 4: Validate / Cleanup
    uLong crc = crc32(0L, outBuf, (uInt)OUT_LEN);
    inflateEnd(&inf);
    inflateEnd(&inf_copy);
    free(input);
    free(compBuf);
    free(outBuf);
    (void)ver;
    (void)inf_ret;
    (void)crc;
    // API sequence test completed successfully
    return 66;
}