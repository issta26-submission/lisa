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
//<ID> 628
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
    compress(compBuf, &compLen, input, src_len);
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inflateInit2_(&inf, 15, ver, (int)sizeof(z_stream));
    inf.next_in = compBuf;
    inf.avail_in = (uInt)compLen;
    inf.next_out = outBuf;
    inf.avail_out = (uInt)OUT_LEN;
    Bytef dict_buf[16];
    memset(dict_buf, 'D', sizeof(dict_buf));
    inflateSetDictionary(&inf, dict_buf, (uInt)sizeof(dict_buf));
    z_stream inf_copy;
    memset(&inf_copy, 0, sizeof(inf_copy));
    inflateCopy(&inf_copy, &inf);
    z_stream def;
    memset(&def, 0, sizeof(def));
    deflateInit_(&def, 6, ver, (int)sizeof(z_stream));

    // step 3: Operate (reset deflate internals and inflate from both streams)
    deflateResetKeep(&def);
    inflate(&inf, 0);
    inflate(&inf_copy, 0);
    uLong checksum = crc32(0L, input, (uInt)INPUT_LEN);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, input, (unsigned int)INPUT_LEN);
    gzclose(gz);

    // step 4: Cleanup
    inflateEnd(&inf);
    inflateEnd(&inf_copy);
    deflateEnd(&def);
    free(input);
    free(compBuf);
    free(outBuf);
    (void)ver;
    (void)checksum;
    // API sequence test completed successfully
    return 66;
}