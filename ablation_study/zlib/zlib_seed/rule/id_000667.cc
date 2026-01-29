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
//<ID> 667
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
    Bytef *compBuf_def = (Bytef *)malloc((size_t)compBound);
    memset(compBuf_def, 0, (size_t)compBound);
    uLongf compLen_def = (uLongf)compBound;
    Bytef *compBuf2 = (Bytef *)malloc((size_t)compBound);
    memset(compBuf2, 0, (size_t)compBound);
    uLongf compLen2 = (uLongf)compBound;

    // step 2: Initialize and run deflate to produce compressed data
    z_stream def;
    memset(&def, 0, sizeof(def));
    def.next_in = input;
    def.avail_in = (uInt)src_len;
    def.next_out = compBuf_def;
    def.avail_out = (uInt)compBound;
    deflateInit_(&def, 6, ver, (int)sizeof(z_stream));
    deflate(&def, 4);
    compLen_def = (uLongf)def.total_out;
    deflateEnd(&def);

    // step 3: Use compress2 on the same input, and write compressed outputs to gz streams
    compress2(compBuf2, &compLen2, input, src_len, 6);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, input, (unsigned int)src_len);
    gzclose(gz);
    gzFile gz_fd = gzdopen(1, "wb");
    gzwrite(gz_fd, compBuf2, (unsigned int)compLen2);
    gzclose(gz_fd);

    // step 4: Reopen for reading, push a byte back, validate and cleanup
    gzFile gz_r = gzopen("test_zlib_api_sequence.gz", "rb");
    int pushed = gzungetc('X', gz_r);
    gzclose(gz_r);
    uLong crc = crc32(0L, input, (uInt)src_len);
    free(input);
    free(compBuf_def);
    free(compBuf2);
    (void)ver;
    (void)compLen_def;
    (void)compLen2;
    (void)pushed;
    (void)crc;
    // API sequence test completed successfully
    return 66;
}