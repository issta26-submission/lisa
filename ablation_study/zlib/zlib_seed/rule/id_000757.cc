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
//<ID> 757
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 512;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'A', (size_t)INPUT_LEN);
    uLong compBound = compressBound((uLong)INPUT_LEN);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf dummyCompLen = (uLongf)compBound;
    Bytef *dictBuf = (Bytef *)malloc(64);
    memset(dictBuf, 'D', 64);

    // step 2: Initialize deflate, set dictionary, perform deflate, and teardown
    z_stream def;
    memset(&def, 0, sizeof(def));
    def.zalloc = NULL;
    def.zfree = NULL;
    def.opaque = NULL;
    def.next_in = input;
    def.avail_in = (uInt)INPUT_LEN;
    def.next_out = compBuf;
    def.avail_out = (uInt)compBound;
    deflateInit_(&def, 6, ver, (int)sizeof(z_stream));
    deflateSetDictionary(&def, dictBuf, (uInt)64);
    deflate(&def, 4); // Z_FINISH == 4
    uLong compLen = def.total_out;
    deflateEnd(&def);

    // step 3: Write original data to gzip file and read it back using gzread
    gzFile wgz = gzopen("tmp_zlib_api.gz", "wb");
    gzwrite(wgz, input, (unsigned int)INPUT_LEN);
    gzclose(wgz);
    gzFile rgz = gzopen("tmp_zlib_api.gz", "rb");
    Bytef *readBuf = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(readBuf, 0, (size_t)INPUT_LEN);
    int readLen = gzread(rgz, readBuf, (unsigned int)INPUT_LEN);
    gzclose(rgz);

    // step 4: Initialize inflate, adjust window bits, inflate compressed data, retrieve dictionary, and cleanup
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inf.zalloc = NULL;
    inf.zfree = NULL;
    inf.opaque = NULL;
    inflateInit_(&inf, ver, (int)sizeof(z_stream));
    inflateReset2(&inf, 15);
    inf.next_in = compBuf;
    inf.avail_in = (uInt)compLen;
    uLong OUT_LEN = INPUT_LEN * 2;
    Bytef *outBuf = (Bytef *)malloc((size_t)OUT_LEN);
    memset(outBuf, 0, (size_t)OUT_LEN);
    inf.next_out = outBuf;
    inf.avail_out = (uInt)OUT_LEN;
    inflate(&inf, 0);
    Bytef *dictOut = (Bytef *)malloc(64);
    memset(dictOut, 0, 64);
    uInt dictOutLen = (uInt)64;
    inflateGetDictionary(&inf, dictOut, &dictOutLen);
    inflateBackEnd(&inf);
    inflateEnd(&inf);

    free(input);
    free(compBuf);
    free(dictBuf);
    free(readBuf);
    free(outBuf);
    free(dictOut);
    (void)ver;
    (void)readLen;
    (void)dummyCompLen;
    // API sequence test completed successfully
    return 66;
}