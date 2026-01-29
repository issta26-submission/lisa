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
//<ID> 750
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers and dictionary
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 512;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'A', (size_t)INPUT_LEN);
    Bytef *dictionary = (Bytef *)malloc(64);
    memset(dictionary, 'D', 64);

    // step 2: Initialize and configure a deflate stream, set dictionary, compress once
    z_stream def;
    memset(&def, 0, sizeof(def));
    def.zalloc = NULL;
    def.zfree = NULL;
    def.opaque = NULL;
    deflateInit_(&def, 6, ver, (int)sizeof(z_stream));
    deflateSetDictionary(&def, dictionary, (uInt)64);
    uLong compBound = deflateBound(&def, (uLong)INPUT_LEN);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    def.next_in = input;
    def.avail_in = (uInt)INPUT_LEN;
    def.next_out = compBuf;
    def.avail_out = (uInt)compBound;
    deflate(&def, 0);
    uLong compLen = def.total_out;
    deflateEnd(&def);

    // step 3: Initialize inflate, adjust window bits, perform inflate, and get dictionary from inflate stream
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
    Bytef *retrievedDict = (Bytef *)malloc(64);
    memset(retrievedDict, 0, 64);
    uInt dictLen = (uInt)64;
    inflateGetDictionary(&inf, retrievedDict, &dictLen);
    inflateEnd(&inf);

    // step 4: Write original data to a gzip file, read it back with gzread, then cleanup
    gzFile wgz = gzopen("tmp_zlib_test.gz", "wb");
    gzwrite(wgz, input, (unsigned int)INPUT_LEN);
    gzclose(wgz);
    gzFile rgz = gzopen("tmp_zlib_test.gz", "rb");
    Bytef *readBuf = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(readBuf, 0, (size_t)INPUT_LEN);
    int readLen = gzread(rgz, readBuf, (unsigned int)INPUT_LEN);
    gzclose(rgz);

    free(input);
    free(dictionary);
    free(compBuf);
    free(outBuf);
    free(retrievedDict);
    free(readBuf);
    (void)ver;
    (void)readLen;
    // API sequence test completed successfully
    return 66;
}