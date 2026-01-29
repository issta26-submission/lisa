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
//<ID> 666
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 512;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'Z', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong bound1 = compressBound(src_len);
    Bytef *compBuf1 = (Bytef *)malloc((size_t)bound1);
    memset(compBuf1, 0, (size_t)bound1);
    uLongf compLen1 = (uLongf)bound1;
    compress2(compBuf1, &compLen1, input, src_len, 6);
    uLong bound2 = compressBound((uLong)compLen1);
    Bytef *compBuf2 = (Bytef *)malloc((size_t)bound2);
    memset(compBuf2, 0, (size_t)bound2);
    z_stream def;
    memset(&def, 0, sizeof(def));
    def.next_in = compBuf1;
    def.avail_in = (uInt)compLen1;
    def.next_out = compBuf2;
    def.avail_out = (uInt)bound2;
    deflateInit_(&def, 6, ver, (int)sizeof(z_stream));
    deflate(&def, 4);
    uLongf compLen2 = (uLongf)def.total_out;
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gw, compBuf2, (unsigned int)compLen2);
    gzclose(gw);
    gzFile gzd = gzopen("test_zlib_api_sequence.gz", "rb");
    int pushed = gzungetc('A', gzd);
    gzclose(gzd);
    deflateEnd(&def);
    free(input);
    free(compBuf1);
    free(compBuf2);
    (void)ver;
    (void)compLen1;
    (void)compLen2;
    (void)pushed;
    return 66;
}