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
//<ID> 625
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

    // step 2: Initialize streams (inflate with windowBits via inflateInit2_, prepare deflate for reset)
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inflateInit2_(&inf, 15, ver, (int)sizeof(z_stream));
    inf.next_in = compBuf;
    inf.avail_in = (uInt)compLen;
    inf.next_out = outBuf;
    inf.avail_out = (uInt)OUT_LEN;
    z_stream inf_copy;
    memset(&inf_copy, 0, sizeof(inf_copy));
    z_stream def;
    memset(&def, 0, sizeof(def));
    deflateInit_(&def, 6, ver, (int)sizeof(z_stream));

    // step 3: Operate — inflate, copy stream, inflate on copy, set dictionary, and reset deflate keep
    int res1 = inflate(&inf, 0);
    inflateCopy(&inf_copy, &inf);
    int res2 = inflate(&inf_copy, 0);
    Bytef dict_buf[32];
    memset(dict_buf, 'D', sizeof(dict_buf));
    inflateSetDictionary(&inf, dict_buf, (uInt)sizeof(dict_buf));
    deflateResetKeep(&def);

    // step 4: Cleanup
    inflateEnd(&inf);
    inflateEnd(&inf_copy);
    deflateEnd(&def);
    free(input);
    free(compBuf);
    free(outBuf);
    (void)ver;
    (void)res1;
    (void)res2;
    // API sequence test completed successfully
    return 66;
}