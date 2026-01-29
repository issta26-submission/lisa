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
//<ID> 765
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
    memset(input, 'Z', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong compBound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    uLong OUT_LEN = INPUT_LEN * 3;
    Bytef *outBuf = (Bytef *)malloc((size_t)OUT_LEN);
    memset(outBuf, 0, (size_t)OUT_LEN);
    Bytef dictBufStatic[32];
    memset(dictBufStatic, 'D', sizeof(dictBufStatic));
    uInt dictLen = (uInt)sizeof(dictBufStatic);

    // step 2: Compress the input data
    compress(compBuf, &compLen, input, src_len);

    // step 3: Initialize inflate stream, set dictionary and get header
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inf.zalloc = NULL;
    inf.zfree = NULL;
    inf.opaque = NULL;
    inflateInit_(&inf, ver, (int)sizeof(z_stream));
    inf.next_in = compBuf;
    inf.avail_in = (uInt)compLen;
    inf.next_out = outBuf;
    inf.avail_out = (uInt)OUT_LEN;
    inflateSetDictionary(&inf, dictBufStatic, dictLen);
    gz_header header;
    memset(&header, 0, sizeof(header));
    inflateGetHeader(&inf, &header);

    // step 4: Perform inflateBack operation, finalize and cleanup
    inflateBack(&inf, (in_func)NULL, NULL, (out_func)NULL, NULL);
    inflateEnd(&inf);
    uLong crc_in = crc32(0L, input, (uInt)src_len);

    free(input);
    free(compBuf);
    free(outBuf);
    (void)ver;
    (void)crc_in;
    (void)header;
    // API sequence test completed successfully
    return 66;
}