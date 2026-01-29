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
//<ID> 637
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
    const uLong OUT_LEN = 256;
    Bytef *outBuf = (Bytef *)malloc((size_t)OUT_LEN);
    memset(outBuf, 0, (size_t)OUT_LEN);

    // step 2: Initialize deflate stream and set gzip header
    z_stream def;
    memset(&def, 0, sizeof(def));
    deflateInit_(&def, 6, ver, (int)sizeof(z_stream));
    static Bytef name_buf[] = "example_name";
    static Bytef comment_buf[] = "example_comment";
    gz_header hdr;
    memset(&hdr, 0, sizeof(hdr));
    hdr.name = name_buf;
    hdr.name_max = (uInt)(sizeof(name_buf));
    hdr.comment = comment_buf;
    hdr.comm_max = (uInt)(sizeof(comment_buf));
    hdr.os = 255;
    deflateSetHeader(&def, &hdr);

    // step 3: Operate - compress with deflate, then inflate and call undermine/back-end
    def.next_in = input;
    def.avail_in = (uInt)src_len;
    def.next_out = compBuf;
    def.avail_out = (uInt)bound;
    deflate(&def, 0);
    uLongf compLenOut = (uLongf)def.total_out;

    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inflateInit_(&inf, ver, (int)sizeof(z_stream));
    inf.next_in = compBuf;
    inf.avail_in = (uInt)compLenOut;
    inf.next_out = outBuf;
    inf.avail_out = (uInt)OUT_LEN;
    inflate(&inf, 0);
    inflateUndermine(&inf, 1);
    inflateBackEnd(&inf);

    // step 4: Validate & Cleanup
    uLong crc = crc32(0L, input, (uInt)src_len);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, input, (unsigned int)src_len);
    gzclose(gz);
    deflateEnd(&def);
    free(input);
    free(compBuf);
    free(outBuf);
    (void)ver;
    (void)crc;
    // API sequence test completed successfully
    return 66;
}