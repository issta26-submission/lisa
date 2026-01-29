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
//<ID> 739
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers, produce compressed data
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 1024;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'X', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong compBound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    compress2(compBuf, &compLen, input, src_len, 6);
    uLong OUT_LEN = INPUT_LEN * 2;
    Bytef *outBuf = (Bytef *)malloc((size_t)OUT_LEN);
    memset(outBuf, 0, (size_t)OUT_LEN);

    // step 2: Initialize and configure a deflate stream, use dictionary and reset-keep
    Bytef *dictionary = (Bytef *)malloc(64);
    memset(dictionary, 'D', 64);
    z_stream def;
    memset(&def, 0, sizeof(def));
    def.zalloc = NULL;
    def.zfree = NULL;
    def.opaque = NULL;
    deflateInit_(&def, 6, ver, (int)sizeof(z_stream));
    deflateSetDictionary(&def, dictionary, (uInt)64);
    deflateResetKeep(&def);
    deflateEnd(&def);

    // step 3: Initialize inflate, perform inflate, check sync point, and teardown inflate backend
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
    inflate(&inf, 0);
    inflateSyncPoint(&inf);
    inflateBackEnd(&inf);
    inflateEnd(&inf);

    // step 4: Write decompressed data to a gz file, cleanup resources
    gzFile gz = gzopen("tmp_zlib_test.gz", "wb");
    gzwrite(gz, outBuf, (unsigned int)src_len);
    gzclose(gz);
    free(dictionary);
    free(input);
    free(compBuf);
    free(outBuf);
    (void)ver;
    // API sequence test completed successfully
    return 66;
}