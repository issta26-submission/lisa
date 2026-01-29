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
//<ID> 638
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
    memset(input, 'Z', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong bound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    compress2(compBuf, &compLen, input, src_len, 6);
    const uLong OUT_LEN = 256;
    Bytef *outBuf = (Bytef *)malloc((size_t)OUT_LEN);
    memset(outBuf, 0, (size_t)OUT_LEN);

    // step 2: Initialize inflate stream, configure and operate
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inflateInit_(&inf, ver, (int)sizeof(z_stream));
    inf.next_in = compBuf;
    inf.avail_in = (uInt)compLen;
    inf.next_out = outBuf;
    inf.avail_out = (uInt)OUT_LEN;
    inflateUndermine(&inf, 1);
    inflate(&inf, 0);
    inflateBackEnd(&inf);

    // step 3: Initialize deflate stream and set gzip header
    z_stream def;
    memset(&def, 0, sizeof(def));
    deflateInit2_(&def, 6, 8, 31, 8, 0, ver, (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, sizeof(head));
    Bytef *namebuf = (Bytef *)malloc(8);
    memset(namebuf, 0, 8);
    namebuf[0] = 't'; namebuf[1] = 'e'; namebuf[2] = 's'; namebuf[3] = 't';
    head.name = namebuf;
    head.name_max = 8;
    deflateSetHeader(&def, &head);
    deflateEnd(&def);

    // step 4: Validate & Cleanup
    uLong crc = crc32(0L, input, (uInt)src_len);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, input, (unsigned int)src_len);
    gzclose(gz);
    inflateEnd(&inf);
    free(input);
    free(compBuf);
    free(outBuf);
    free(namebuf);
    (void)ver;
    (void)crc;
    // API sequence test completed successfully
    return 66;
}