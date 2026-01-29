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
//<ID> 620
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
    memset(input, 'Z', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong bound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    compress2(compBuf, &compLen, input, src_len, 6);

    // step 2: Initialize streams and copy
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inflateInit2_(&inf, 15, ver, (int)sizeof(z_stream));
    inf.next_in = compBuf;
    inf.avail_in = (uInt)compLen;
    const uLong OUT_LEN = 512;
    Bytef *outBuf = (Bytef *)malloc((size_t)OUT_LEN);
    memset(outBuf, 0, (size_t)OUT_LEN);
    inf.next_out = outBuf;
    inf.avail_out = (uInt)OUT_LEN;
    z_stream inf_copy;
    memset(&inf_copy, 0, sizeof(inf_copy));
    inflateCopy(&inf_copy, &inf);

    // step 3: Configure dictionary, deflate reset-keep and perform inflate
    Bytef dict[4];
    dict[0] = 'Z'; dict[1] = 'Z'; dict[2] = 'Z'; dict[3] = 'Z';
    inflateSetDictionary(&inf, dict, (uInt)4);
    z_stream def;
    memset(&def, 0, sizeof(def));
    deflateInit_(&def, 6, ver, (int)sizeof(z_stream));
    deflateResetKeep(&def);
    inflate(&inf_copy, 0);

    // step 4: Validate / write and cleanup
    uLong out_total = inf_copy.total_out;
    uInt write_len = (uInt)out_total;
    uLong crc = crc32(0L, outBuf, write_len);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, outBuf, write_len);
    gzclose(gz);
    inflateEnd(&inf_copy);
    inflateEnd(&inf);
    deflateEnd(&def);
    free(input);
    free(compBuf);
    free(outBuf);
    (void)ver;
    (void)crc;
    // API sequence test completed successfully
    return 66;
}