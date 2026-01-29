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
//<ID> 624
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 128;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'A', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong bound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    compress2(compBuf, &compLen, input, src_len, 6);
    const uLong OUT_LEN = INPUT_LEN + 64;
    Bytef *outBuf = (Bytef *)malloc((size_t)OUT_LEN);
    memset(outBuf, 0, (size_t)OUT_LEN);

    // step 2: Initialize streams and configure
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inflateInit2_(&inf, 15, ver, (int)sizeof(z_stream));
    inf.next_in = compBuf;
    inf.avail_in = (uInt)compLen;
    inf.next_out = outBuf;
    inf.avail_out = (uInt)OUT_LEN;
    z_stream defl;
    memset(&defl, 0, sizeof(defl));
    deflateInit_(&defl, 6, ver, (int)sizeof(z_stream));
    deflateResetKeep(&defl);

    // step 3: Operate (copy/inflate/configure dictionary)
    z_stream inf_copy;
    memset(&inf_copy, 0, sizeof(inf_copy));
    inflateCopy(&inf_copy, &inf);
    Bytef dict_buf[] = { 'D', 'I', 'C', 'T' };
    uInt dict_len = (uInt)sizeof(dict_buf);
    inflateSetDictionary(&inf_copy, dict_buf, dict_len);
    inflate(&inf, 0);
    inflate(&inf_copy, 0);

    // step 4: Validate & cleanup
    uLong crc = crc32(0L, input, (uInt)src_len);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, input, (unsigned int)src_len);
    gzclose(gz);
    inflateEnd(&inf);
    inflateEnd(&inf_copy);
    deflateEnd(&defl);
    free(input);
    free(compBuf);
    free(outBuf);
    (void)ver;
    (void)crc;
    // API sequence test completed successfully
    return 66;
}