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
//<ID> 668
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
    Bytef *defOut = (Bytef *)malloc((size_t)(compBound + 64));
    memset(defOut, 0, (size_t)(compBound + 64));

    // step 2: One-shot compression with compress2
    int comp_rc = compress2(compBuf, &compLen, input, src_len, 6);

    // step 3: Initialize and run deflate to produce another compressed stream
    z_stream def;
    memset(&def, 0, sizeof(def));
    def.next_in = input;
    def.avail_in = (uInt)src_len;
    def.next_out = defOut;
    def.avail_out = (uInt)(compBound + 64);
    deflateInit_(&def, 6, ver, (int)sizeof(z_stream));
    deflate(&def, 4);
    uLong defOutLen = def.total_out;

    // step 4: Persist compressed data using gzopen, then reopen via gzdopen and use gzungetc, cleanup
    gzFile gzw = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gzw, compBuf, (unsigned int)compLen);
    gzclose(gzw);
    int fd = open("test_zlib_api_sequence.gz", 0);
    gzFile gzr = gzdopen(fd, "rb");
    gzungetc((int)'A', gzr);
    gzclose(gzr);

    deflateEnd(&def);
    free(input);
    free(compBuf);
    free(defOut);
    (void)ver;
    (void)comp_rc;
    (void)defOutLen;
    // API sequence test completed successfully
    return 66;
}