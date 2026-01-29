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
//<ID> 665
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 256;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'A', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong compBound = compressBound(src_len);
    Bytef *comp1 = (Bytef *)malloc((size_t)compBound);
    memset(comp1, 0, (size_t)compBound);
    uLongf comp1Len = (uLongf)compBound;

    // step 2: High-level compress using compress2
    compress2(comp1, &comp1Len, input, src_len, 6);

    // step 3: Initialize and run deflate (low-level) on the original input
    uLong OUT_LEN = compBound * 2 + 64;
    Bytef *comp2 = (Bytef *)malloc((size_t)OUT_LEN);
    memset(comp2, 0, (size_t)OUT_LEN);
    uLongf comp2Len = (uLongf)OUT_LEN;
    z_stream def;
    memset(&def, 0, sizeof(def));
    def.next_in = input;
    def.avail_in = (uInt)src_len;
    def.next_out = comp2;
    def.avail_out = (uInt)OUT_LEN;
    deflateInit_(&def, 6, ver, (int)sizeof(z_stream));
    deflate(&def, 4);
    comp2Len = (uLongf)def.total_out;
    deflateEnd(&def);

    // step 4: Write compressed result to a gzip file, demonstrate gzungetc and gzdopen, then cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, comp2, (unsigned int)comp2Len);
    gzclose(gz);
    gzFile gz2 = gzopen("test_zlib_api_sequence.gz", "rb");
    int ungot = gzungetc((int)comp2[0], gz2);
    gzFile gz3 = gzdopen(1, "wb");
    gzwrite(gz3, input, (unsigned int)src_len);
    gzclose(gz3);
    gzclose(gz2);
    free(input);
    free(comp1);
    free(comp2);
    (void)ver;
    (void)comp1Len;
    (void)comp2Len;
    (void)ungot;
    // API sequence test completed successfully
    return 66;
}