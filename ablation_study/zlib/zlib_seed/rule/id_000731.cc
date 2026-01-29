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
//<ID> 731
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
    memset(input, 'X', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong compBound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    uLong OUT_LEN = INPUT_LEN * 2;
    Bytef *outBuf = (Bytef *)malloc((size_t)OUT_LEN);
    memset(outBuf, 0, (size_t)OUT_LEN);

    // step 2: Compress input (produce zlib-compatible data) and initialize a deflate stream then reset-keep it
    compress2(compBuf, &compLen, input, src_len, 6);
    z_stream def;
    memset(&def, 0, sizeof(def));
    def.zalloc = NULL;
    def.zfree = NULL;
    def.opaque = NULL;
    def.next_in = NULL;
    def.avail_in = 0;
    def.next_out = NULL;
    def.avail_out = 0;
    deflateInit_(&def, 6, ver, (int)sizeof(z_stream));
    deflateResetKeep(&def);
    deflateEnd(&def);

    // step 3: Initialize inflate, perform inflate, check sync point, and use inflateBackInit_/inflateBackEnd
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
    z_stream back;
    memset(&back, 0, sizeof(back));
    unsigned char *window = (unsigned char *)malloc((size_t)32768);
    memset(window, 0, (size_t)32768);
    inflateBackInit_(&back, 15, window, ver, (int)sizeof(z_stream));
    inflateBackEnd(&back);
    inflateEnd(&inf);
    free(window);

    // step 4: Write original input to a gz file, cleanup buffers and file handles
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, (voidpc)input, (unsigned int)INPUT_LEN);
    gzclose(gz);
    free(input);
    free(compBuf);
    free(outBuf);

    // API sequence test completed successfully
    return 66;
}