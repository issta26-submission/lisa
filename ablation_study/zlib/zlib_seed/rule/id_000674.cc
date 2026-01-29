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
//<ID> 674
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
    memset(input, 'Z', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong compBound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    const uLong OUT_LEN = INPUT_LEN * 2;
    Bytef *outBuf = (Bytef *)malloc((size_t)OUT_LEN);
    memset(outBuf, 0, (size_t)OUT_LEN);
    Bytef *uncompBuf = (Bytef *)malloc((size_t)OUT_LEN);
    memset(uncompBuf, 0, (size_t)OUT_LEN);

    // step 2: Initialize, configure and run deflate
    z_stream def;
    memset(&def, 0, sizeof(def));
    def.next_in = input;
    def.avail_in = (uInt)src_len;
    def.next_out = compBuf;
    def.avail_out = (uInt)compBound;
    deflateInit_(&def, 6, ver, (int)sizeof(z_stream));
    deflate(&def, 4);
    uLong compSize = def.total_out;
    deflateEnd(&def);

    // step 3: Decompress using uncompress2 and inflate, then cleanup inflate backend
    uLongf destLen = (uLongf)OUT_LEN;
    uLong compSizeVar = compSize;
    int rc_uncomp = uncompress2(uncompBuf, &destLen, compBuf, &compSizeVar);
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inflateInit_(&inf, ver, (int)sizeof(z_stream));
    inf.next_in = compBuf;
    inf.avail_in = (uInt)compSize;
    inf.next_out = outBuf;
    inf.avail_out = (uInt)OUT_LEN;
    inflate(&inf, 0);
    inflateBackEnd(&inf);
    inflateEnd(&inf);

    // step 4: Use gz APIs, validate and cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int putc_ret = gzputc(gz, (int)input[0]);
    gzclose(gz);
    free(input);
    free(compBuf);
    free(outBuf);
    free(uncompBuf);
    (void)ver;
    (void)compSize;
    (void)rc_uncomp;
    (void)putc_ret;
    // API sequence test completed successfully
    return 66;
}