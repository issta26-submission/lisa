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
//<ID> 636
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
    const uLong OUT_LEN = 256;
    Bytef *outBuf = (Bytef *)malloc((size_t)OUT_LEN);
    memset(outBuf, 0, (size_t)OUT_LEN);

    // step 2: Initialize deflate stream with gzip wrapper and set header, then compress
    z_stream def;
    memset(&def, 0, sizeof(def));
    deflateInit2_(&def, 6, 8, 31, 8, 0, ver, (int)sizeof(z_stream));
    gz_header header;
    memset(&header, 0, sizeof(header));
    Bytef *nameBuf = (Bytef *)malloc(16);
    memset(nameBuf, 0, 16);
    memcpy(nameBuf, "testname", 8);
    header.name = nameBuf;
    header.name_max = (uInt)16;
    deflateSetHeader(&def, &header);
    uLong bound = deflateBound(&def, src_len);
    Bytef *defBuf = (Bytef *)malloc((size_t)bound);
    memset(defBuf, 0, (size_t)bound);
    def.next_in = input;
    def.avail_in = (uInt)src_len;
    def.next_out = defBuf;
    def.avail_out = (uInt)bound;
    deflate(&def, 4);
    uLong compLen = def.total_out;
    deflateEnd(&def);

    // step 3: Initialize inflate stream, inflate compressed data, call undermine and backend
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inflateInit2_(&inf, 31, ver, (int)sizeof(z_stream));
    inf.next_in = defBuf;
    inf.avail_in = (uInt)compLen;
    inf.next_out = outBuf;
    inf.avail_out = (uInt)OUT_LEN;
    inflate(&inf, 0);
    inflateUndermine(&inf, 1);
    inflateBackEnd(&inf);
    inflateEnd(&inf);

    // step 4: Cleanup
    free(input);
    free(defBuf);
    free(outBuf);
    free(nameBuf);
    (void)ver;
    // API sequence test completed successfully
    return 66;
}