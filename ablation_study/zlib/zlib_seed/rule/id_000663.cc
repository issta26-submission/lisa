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
//<ID> 663
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
    uLong bound = compressBound(src_len);
    Bytef *comp1 = (Bytef *)malloc((size_t)bound);
    memset(comp1, 0, (size_t)bound);
    uLongf comp1Len = (uLongf)bound;

    // step 2: Compress with compress2 (simple one-shot) and then further compress via deflate
    compress2(comp1, &comp1Len, input, src_len, 6);

    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    strm.next_in = comp1;
    strm.avail_in = (uInt)comp1Len;
    const uLong OUT_LEN = src_len * 2 + 1024;
    Bytef *comp2 = (Bytef *)malloc((size_t)OUT_LEN);
    memset(comp2, 0, (size_t)OUT_LEN);
    strm.next_out = comp2;
    strm.avail_out = (uInt)OUT_LEN;
    deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));
    deflate(&strm, 4);
    uLongf comp2Len = (uLongf)strm.total_out;

    // step 3: Write original data into a gz file, reopen for reading, use gzungetc and gzdopen
    gzFile gzw = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gzw, input, (unsigned int)src_len);
    gzclose(gzw);

    gzFile gzr = gzopen("test_zlib_api_sequence.gz", "rb");
    int first_char = gzgetc(gzr);
    gzungetc(first_char, gzr);

    gzFile gzd = gzdopen(1, "wb");
    gzclose(gzd);

    // step 4: Cleanup & finalization
    gzclose(gzr);
    deflateEnd(&strm);
    free(input);
    free(comp1);
    free(comp2);
    (void)ver;
    (void)comp1Len;
    (void)comp2Len;
    (void)first_char;
    // API sequence test completed successfully
    return 66;
}