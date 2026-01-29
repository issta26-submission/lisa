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
//<ID> 660
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
    uLongf compLen = (uLongf)compBound;

    // step 2: Compress with compress2
    compress2(compBuf, &compLen, input, src_len, 6);

    // step 3: Initialize and run deflate on the compressed data (stream API usage)
    z_stream def;
    memset(&def, 0, sizeof(def));
    def.next_in = compBuf;
    def.avail_in = (uInt)compLen;
    uLong DEF_OUT_LEN = compLen + 256;
    Bytef *defOut = (Bytef *)malloc((size_t)DEF_OUT_LEN);
    memset(defOut, 0, (size_t)DEF_OUT_LEN);
    def.next_out = defOut;
    def.avail_out = (uInt)DEF_OUT_LEN;
    deflateInit_(&def, 6, ver, (int)sizeof(z_stream));
    deflate(&def, 4);
    uLong defTotalOut = def.total_out;
    deflateEnd(&def);

    // step 4: Use gzopen, gzdopen and gzungetc, then cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, input, (unsigned int)src_len);
    int push1 = gzungetc('A', gz);
    gzclose(gz);
    gzFile gz2 = gzdopen(1, "wb");
    int push2 = gzungetc('B', gz2);
    gzclose(gz2);

    free(input);
    free(compBuf);
    free(defOut);
    (void)ver;
    (void)compLen;
    (void)defTotalOut;
    (void)push1;
    (void)push2;
    // API sequence test completed successfully
    return 66;
}