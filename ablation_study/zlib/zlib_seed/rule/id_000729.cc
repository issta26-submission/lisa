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
//<ID> 729
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
    uLong src_len = (uLong)INPUT_LEN;
    uLong compBound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    uLong OUT_LEN = INPUT_LEN * 2;
    Bytef *outBuf = (Bytef *)malloc((size_t)OUT_LEN);
    memset(outBuf, 0, (size_t)OUT_LEN);
    Bytef *dictionary = (Bytef *)malloc(64);
    memset(dictionary, 'D', 64);

    // step 2: Compress using the simple compress2 API (produces zlib-compatible data)
    compress2(compBuf, &compLen, input, src_len, 6);

    // step 3: Initialize a deflate stream, set a dictionary on it, then clean it up
    z_stream def;
    memset(&def, 0, sizeof(def));
    def.zalloc = NULL;
    def.zfree = NULL;
    def.opaque = NULL;
    def.next_in = NULL;
    def.avail_in = 0;
    deflateInit_(&def, 6, ver, (int)sizeof(z_stream));
    deflateSetDictionary(&def, dictionary, (uInt)64);
    deflateEnd(&def);

    // step 4: Initialize inflate, use uncompress to restore data, then cleanup all resources
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inf.zalloc = NULL;
    inf.zfree = NULL;
    inf.opaque = NULL;
    inflateInit_(&inf, ver, (int)sizeof(z_stream));
    uLongf destLen = (uLongf)OUT_LEN;
    uncompress(outBuf, &destLen, compBuf, (uLong)compLen);
    inflateEnd(&inf);

    free(dictionary);
    free(input);
    free(compBuf);
    free(outBuf);

    // API sequence test completed successfully
    return 66;
}