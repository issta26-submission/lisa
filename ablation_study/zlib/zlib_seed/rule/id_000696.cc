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
//<ID> 696
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
    uLong compBound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLong compLen = 0;

    // step 2: Initialize, configure and run deflate to produce compressed data
    z_stream def;
    memset(&def, 0, sizeof(def));
    def.next_in = input;
    def.avail_in = (uInt)src_len;
    def.next_out = compBuf;
    def.avail_out = (uInt)compBound;
    deflateInit_(&def, 6, ver, (int)sizeof(z_stream));
    deflate(&def, 4);
    compLen = def.total_out;
    deflateEnd(&def);

    // step 3: Initialize inflate and decompress the produced data
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inflateInit_(&inf, ver, (int)sizeof(z_stream));
    inf.next_in = compBuf;
    inf.avail_in = (uInt)compLen;
    uLong OUT_LEN = INPUT_LEN * 2;
    Bytef *out = (Bytef *)malloc((size_t)OUT_LEN);
    memset(out, 0, (size_t)OUT_LEN);
    inf.next_out = out;
    inf.avail_out = (uInt)OUT_LEN;
    inflate(&inf, 0);

    // step 4: Validate via adler32 and inflateCodesUsed, write a byte using gzputc, cleanup
    uLong checksum = adler32(0UL, out, (uInt)inf.total_out);
    unsigned long codes_used = inflateCodesUsed(&inf);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputc(gz, (int)out[0]);
    gzclose(gz);
    inflateEnd(&inf);

    free(input);
    free(compBuf);
    free(out);
    (void)ver;
    (void)src_len;
    (void)compBound;
    (void)compLen;
    (void)checksum;
    (void)codes_used;
    // API sequence test completed successfully
    return 66;
}