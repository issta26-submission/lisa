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
//<ID> 803
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare input, compute CRC parts and combine
    const char *ver = zlibVersion();
    const uLong SRC_LEN = 1024;
    Bytef *input = (Bytef *)malloc((size_t)SRC_LEN);
    memset(input, 'A', (size_t)SRC_LEN);
    uInt half_len = (uInt)(SRC_LEN / 2);
    uLong crc_part1 = crc32(0L, input, half_len);
    uLong crc_part2 = crc32(0L, input + half_len, (uInt)(SRC_LEN - half_len));
    uLong combined_crc = crc32_combine(crc_part1, crc_part2, (off_t)(SRC_LEN - half_len));

    // step 2: Initialize deflate stream, compute bound, and compress using compress2
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, SRC_LEN);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    uLong comp_len = bound;
    compress2(comp_buf, &comp_len, input, SRC_LEN, 6);

    // step 3: Initialize inflateBack stream, set dictionary, decompress via uncompress, then end inflateBack
    z_stream backstrm;
    memset(&backstrm, 0, sizeof(backstrm));
    backstrm.zalloc = NULL;
    backstrm.zfree = NULL;
    backstrm.opaque = NULL;
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&backstrm, 15, window, ver, (int)sizeof(z_stream));
    uInt dict_len = 16;
    inflateSetDictionary(&backstrm, input, dict_len);
    Bytef *out_buf = (Bytef *)malloc((size_t)(SRC_LEN * 2));
    uLong out_len = SRC_LEN * 2;
    uncompress(out_buf, &out_len, comp_buf, comp_len);
    inflateBackEnd(&backstrm);

    // step 4: Cleanup deflate, free resources, and finalize
    deflateEnd(&dstrm);
    free(input);
    free(comp_buf);
    free(out_buf);
    (void)ver;
    (void)crc_part1;
    (void)crc_part2;
    (void)combined_crc;
    (void)bound;
    (void)comp_len;
    (void)out_len;
    // API sequence test completed successfully
    return 66;
}