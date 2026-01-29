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
//<ID> 627
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
    uLong bound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    const uLong OUT_LEN = 512;
    Bytef *outBuf = (Bytef *)malloc((size_t)OUT_LEN);
    memset(outBuf, 0, (size_t)OUT_LEN);

    // step 2: Compress input and initialize streams
    compress2(compBuf, &compLen, input, src_len, 6);
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inflateInit2_(&inf, 15, ver, (int)sizeof(z_stream));
    inf.next_in = compBuf;
    inf.avail_in = (uInt)compLen;
    inf.next_out = outBuf;
    inf.avail_out = (uInt)OUT_LEN;

    // step 3: Inflate, copy stream, set dictionary, and deflate reset keep
    inflate(&inf, 0);
    z_stream inf_copy;
    memset(&inf_copy, 0, sizeof(inf_copy));
    inflateCopy(&inf_copy, &inf);
    const uInt dict_len = 32U;
    Bytef *dict = (Bytef *)malloc((size_t)dict_len);
    memset(dict, 'D', (size_t)dict_len);
    inflateSetDictionary(&inf_copy, dict, dict_len);
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    deflateResetKeep(&dstrm);

    // step 4: Validation & cleanup
    uLong crc1 = crc32(0L, input, (uInt)(INPUT_LEN / 2));
    uLong crc2 = crc32(0L, input + (INPUT_LEN / 2), (uInt)(INPUT_LEN - (INPUT_LEN / 2)));
    uLong combined_crc = crc32_combine(crc1, crc2, (off_t)(INPUT_LEN - (INPUT_LEN / 2)));
    inflateEnd(&inf);
    inflateEnd(&inf_copy);
    deflateEnd(&dstrm);
    free(input);
    free(compBuf);
    free(outBuf);
    free(dict);
    (void)combined_crc;
    // API sequence test completed successfully
    return 66;
}