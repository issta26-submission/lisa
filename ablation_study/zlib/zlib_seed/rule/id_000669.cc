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
//<ID> 669
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
    memset(input, 'X', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong compBound1 = compressBound(src_len);
    Bytef *compBuf1 = (Bytef *)malloc((size_t)compBound1);
    memset(compBuf1, 0, (size_t)compBound1);
    uLongf destLen1 = (uLongf)compBound1;
    compress2(compBuf1, &destLen1, input, src_len, 6);

    // step 2: Initialize, configure and run deflate
    uLong compBound2 = compressBound(src_len);
    Bytef *compBuf2 = (Bytef *)malloc((size_t)compBound2);
    memset(compBuf2, 0, (size_t)compBound2);
    z_stream def;
    memset(&def, 0, sizeof(def));
    def.next_in = input;
    def.avail_in = (uInt)src_len;
    def.next_out = compBuf2;
    def.avail_out = (uInt)compBound2;
    deflateInit_(&def, 6, ver, (int)sizeof(z_stream));
    deflate(&def, 4);
    uLongf deflated_len = (uLongf)def.total_out;
    deflateEnd(&def);

    // step 3: Use gzopen/gzdopen and gzungetc to exercise gz APIs
    gzFile gzw = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gzw, compBuf1, (unsigned int)destLen1);
    gzclose(gzw);
    gzFile gzr = gzopen("test_zlib_api_sequence.gz", "rb");
    int first = gzgetc(gzr);
    gzungetc(first, gzr);
    gzclose(gzr);
    gzFile gz_stdout = gzdopen(1, "wb");
    gzwrite(gz_stdout, input, (unsigned int)src_len);
    gzflush(gz_stdout, 2);
    gzclose(gz_stdout);

    // step 4: Validate & Cleanup
    uLong crc = crc32(0L, input, (uInt)src_len);
    free(input);
    free(compBuf1);
    free(compBuf2);
    (void)ver;
    (void)deflated_len;
    (void)crc;
    (void)first;
    // API sequence test completed successfully
    return 66;
}