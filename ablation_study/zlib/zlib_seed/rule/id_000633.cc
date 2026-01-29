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
//<ID> 633
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
    compress(compBuf, &compLen, input, src_len);
    const uLong OUT_LEN = 512;
    Bytef *outBuf = (Bytef *)malloc((size_t)OUT_LEN);
    memset(outBuf, 0, (size_t)OUT_LEN);

    // step 2: Initialize deflate stream and set gzip header
    z_stream def;
    memset(&def, 0, sizeof(def));
    deflateInit2_(&def, 6, 8, 31, 8, 0, ver, (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, sizeof(head));
    const char hdr_name[] = "fuz";
    head.name = (Bytef *)malloc(sizeof(hdr_name));
    memcpy(head.name, hdr_name, sizeof(hdr_name));
    head.name_max = (uInt)(sizeof(hdr_name) - 1);
    deflateSetHeader(&def, &head);
    deflateEnd(&def);
    free(head.name);

    // step 3: Initialize inflate stream, configure undermine, perform inflate and backend
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
    inflateEnd(&inf);

    // step 4: Validate & Cleanup
    uLong ad = adler32(0L, input, (uInt)src_len);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, input, (unsigned int)src_len);
    gzflush(gz, 2);
    gzclose(gz);
    free(input);
    free(compBuf);
    free(outBuf);
    (void)ver;
    (void)ad;
    // API sequence test completed successfully
    return 66;
}