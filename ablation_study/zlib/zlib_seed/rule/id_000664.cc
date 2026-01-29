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
//<ID> 664
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers and run a one-shot compress
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 512;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'X', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong compBound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int cstatus = compress2(compBuf, &compLen, input, src_len, 6);

    // step 2: Initialize, configure and run deflate on the original input
    z_stream def;
    memset(&def, 0, sizeof(def));
    Bytef *defBuf = (Bytef *)malloc((size_t)compBound);
    memset(defBuf, 0, (size_t)compBound);
    def.next_in = input;
    def.avail_in = (uInt)src_len;
    def.next_out = defBuf;
    def.avail_out = (uInt)compBound;
    int dinit = deflateInit_(&def, 6, ver, (int)sizeof(z_stream));
    int dstatus = deflate(&def, 4);
    uLong defOut = def.total_out;
    int dend = deflateEnd(&def);

    // step 3: Write compressed output to a .gz file, then re-open with gzdopen and push a character back
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw = gzwrite(gz, defBuf, (unsigned int)defOut);
    int gc = gzclose(gz);
    FILE *f = fopen("test_zlib_api_sequence.gz", "rb");
    int fd = fileno(f);
    int dupfd = dup(fd);
    gzFile gz2 = gzdopen(dupfd, "rb");
    int pushed = gzungetc('A', gz2);
    int gz2c = gzclose(gz2);
    int fc = fclose(f);

    // step 4: Validate & Cleanup
    uLong crc = crc32(0L, input, (uInt)src_len);
    free(input);
    free(compBuf);
    free(defBuf);
    (void)ver;
    (void)cstatus;
    (void)dinit;
    (void)dstatus;
    (void)dend;
    (void)gw;
    (void)gc;
    (void)gz2c;
    (void)fc;
    (void)pushed;
    (void)crc;
    // API sequence test completed successfully
    return 66;
}