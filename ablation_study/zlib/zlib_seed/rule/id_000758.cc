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
//<ID> 758
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare input, produce compressed data with compress2
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 1024;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'A', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong compBound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    compress2(compBuf, &compLen, input, src_len, 6);

    // step 2: Initialize a deflate stream, set up and run deflate on the original input (demonstrate deflate usage)
    Bytef *defOut = (Bytef *)malloc((size_t)compBound);
    memset(defOut, 0, (size_t)compBound);
    z_stream def;
    memset(&def, 0, sizeof(def));
    def.zalloc = NULL;
    def.zfree = NULL;
    def.opaque = NULL;
    def.next_in = input;
    def.avail_in = (uInt)src_len;
    def.next_out = defOut;
    def.avail_out = (uInt)compBound;
    deflateInit_(&def, 6, ver, (int)sizeof(z_stream));
    deflate(&def, 0);
    deflateEnd(&def);

    // step 3: Initialize inflate, adjust window bits with inflateReset2, inflate the compressed buffer, and retrieve dictionary
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inf.zalloc = NULL;
    inf.zfree = NULL;
    inf.opaque = NULL;
    inflateInit_(&inf, ver, (int)sizeof(z_stream));
    inflateReset2(&inf, 15);
    uLong OUT_LEN = INPUT_LEN * 2;
    Bytef *outBuf = (Bytef *)malloc((size_t)OUT_LEN);
    memset(outBuf, 0, (size_t)OUT_LEN);
    inf.next_in = compBuf;
    inf.avail_in = (uInt)compLen;
    inf.next_out = outBuf;
    inf.avail_out = (uInt)OUT_LEN;
    inflate(&inf, 0);
    Bytef *dictBuf = (Bytef *)malloc(64);
    memset(dictBuf, 0, 64);
    uInt dictLen = (uInt)64;
    inflateGetDictionary(&inf, dictBuf, &dictLen);
    inflateEnd(&inf);

    // step 4: Write decompressed data to a gzip file, read it back with gzread, cleanup resources
    gzFile wgz = gzopen("tmp_zlib_test.gz", "wb");
    gzwrite(wgz, outBuf, (unsigned int)src_len);
    gzclose(wgz);
    gzFile rgz = gzopen("tmp_zlib_test.gz", "rb");
    Bytef *readBuf = (Bytef *)malloc((size_t)src_len);
    memset(readBuf, 0, (size_t)src_len);
    int readLen = gzread(rgz, readBuf, (unsigned int)src_len);
    gzclose(rgz);

    free(input);
    free(compBuf);
    free(defOut);
    free(outBuf);
    free(dictBuf);
    free(readBuf);
    (void)ver;
    (void)readLen;
    // API sequence test completed successfully
    return 66;
}