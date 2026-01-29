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
//<ID> 622
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
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

    // step 2: Initialize streams
    const char *ver = zlibVersion();
    z_stream src_inf;
    z_stream copy_inf;
    z_stream def_strm;
    memset(&src_inf, 0, sizeof(src_inf));
    memset(&copy_inf, 0, sizeof(copy_inf));
    memset(&def_strm, 0, sizeof(def_strm));
    inflateInit2_(&src_inf, 15, ver, (int)sizeof(z_stream));
    src_inf.next_in = compBuf;
    src_inf.avail_in = (uInt)compLen;
    src_inf.next_out = outBuf;
    src_inf.avail_out = (uInt)OUT_LEN;
    inflateCopy(&copy_inf, &src_inf);

    // step 3: Configure and operate
    inflateSetDictionary(&copy_inf, input, (uInt)32);
    inflate(&copy_inf, 0);
    deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    deflateResetKeep(&def_strm);
    deflateEnd(&def_strm);

    // step 4: Cleanup and finalization
    inflateEnd(&copy_inf);
    inflateEnd(&src_inf);
    free(input);
    free(compBuf);
    free(outBuf);

    // API sequence test completed successfully
    (void)ver;
    return 66;
}