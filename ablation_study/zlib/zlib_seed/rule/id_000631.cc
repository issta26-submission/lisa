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
//<ID> 631
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
    const uLong OUT_LEN = 512;
    Bytef *outBuf = (Bytef *)malloc((size_t)OUT_LEN);
    memset(outBuf, 0, (size_t)OUT_LEN);

    // step 2: Initialize deflate stream and set gzip header, then deflate once
    z_stream def;
    memset(&def, 0, sizeof(def));
    deflateInit_(&def, 6, ver, (int)sizeof(z_stream));
    gz_header hdr;
    memset(&hdr, 0, sizeof(hdr));
    hdr.text = 0;
    hdr.time = 0;
    hdr.xflags = 0;
    hdr.os = 255;
    hdr.name = (Bytef *)malloc(16);
    memset(hdr.name, 0, 16);
    memcpy(hdr.name, "testname", 8);
    hdr.name_max = 15;
    (void)deflateSetHeader; /* reference function symbol to avoid unused warnings */
    deflateSetHeader(&def, &hdr);
    def.next_in = input;
    def.avail_in = (uInt)src_len;
    def.next_out = compBuf;
    def.avail_out = (uInt)bound;
    deflate(&def, 0);
    deflateEnd(&def);

    // step 3: Initialize inflate stream, operate, call undermine and backend
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inflateInit_(&inf, ver, (int)sizeof(z_stream));
    inf.next_in = compBuf;
    inf.avail_in = (uInt)def.total_out;
    inf.next_out = outBuf;
    inf.avail_out = (uInt)OUT_LEN;
    inflate(&inf, 0);
    inflateUndermine(&inf, 1);
    inflateBackEnd(&inf);
    inflateEnd(&inf);

    // step 4: Validate actions and cleanup
    uLong crc = crc32(0L, input, (uInt)src_len);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, input, (unsigned int)src_len);
    gzclose(gz);
    free(hdr.name);
    free(input);
    free(compBuf);
    free(outBuf);
    (void)ver;
    (void)crc;
    // API sequence test completed successfully
    return 66;
}