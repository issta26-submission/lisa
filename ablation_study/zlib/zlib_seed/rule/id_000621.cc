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
//<ID> 621
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 160;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'A', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong bound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    Bytef *outBuf = (Bytef *)malloc((size_t)(src_len * 2));
    memset(outBuf, 0, (size_t)(src_len * 2));
    Bytef *dict = (Bytef *)malloc(32);
    memset(dict, 'D', 32);

    // step 2: Compress input and initialize inflate stream with specific windowBits
    compress2(compBuf, &compLen, input, src_len, 6);
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inflateInit2_(&inf, 15, ver, (int)sizeof(z_stream));
    inf.next_in = compBuf;
    inf.avail_in = (uInt)compLen;
    inf.next_out = outBuf;
    inf.avail_out = (uInt)(src_len * 2);
    inflateSetDictionary(&inf, dict, (uInt)32);

    // step 3: Operate: inflate, copy inflater, and reset a deflate stream keep state
    inflate(&inf, 0);
    z_stream inf2;
    memset(&inf2, 0, sizeof(inf2));
    inflateCopy(&inf2, &inf);
    inflate(&inf2, 0);
    z_stream def;
    memset(&def, 0, sizeof(def));
    deflateInit_(&def, 6, ver, (int)sizeof(z_stream));
    deflateResetKeep(&def);

    // step 4: Validate & Cleanup
    uLong crc1 = crc32(0L, input, (uInt)80);
    uLong crc2 = crc32(0L, input + 80, (uInt)(src_len - 80));
    uLong combined_crc = crc32_combine(crc1, crc2, (off_t)(src_len - 80));
    inflateEnd(&inf);
    inflateEnd(&inf2);
    deflateEnd(&def);
    free(input);
    free(compBuf);
    free(outBuf);
    free(dict);
    (void)ver;
    (void)combined_crc;
    // API sequence test completed successfully
    return 66;
}