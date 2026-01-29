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
//<ID> 107
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream src_strm;
    z_stream copy_strm;
    memset(&src_strm, 0, sizeof(z_stream));
    memset(&copy_strm, 0, sizeof(z_stream));
    const char src_c[] = "Example payload for zlib: compress -> inflateInit_ -> inflateCopy -> gzwrite/gzputc";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    const char *version = zlibVersion();

    // step 2: Setup / Configure (compress source and initialize inflate source stream)
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);
    int rc_inf_init = inflateInit_(&src_strm, version, (int)sizeof(z_stream));
    Bytef *outBuf = (Bytef *)malloc((size_t)sourceLen);
    memset(outBuf, 0, (size_t)sourceLen);
    src_strm.next_in = compBuf;
    src_strm.avail_in = (uInt)compLen;
    src_strm.next_out = outBuf;
    src_strm.avail_out = (uInt)sourceLen;

    // step 3: Operate (copy inflate state, write compressed data to gz, write a single char)
    int rc_copy = inflateCopy(&copy_strm, &src_strm);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_write = gzwrite(gz, compBuf, (unsigned int)compLen);
    int rc_gz_putc = gzputc(gz, 'A');
    int rc_inf_copy = inflate(&copy_strm, 0);

    // step 4: Cleanup & finalize
    (void)rc_comp; (void)rc_inf_init; (void)rc_copy; (void)rc_gz_write; (void)rc_gz_putc; (void)rc_inf_copy;
    gzclose(gz);
    inflateEnd(&copy_strm);
    inflateEnd(&src_strm);
    free(compBuf);
    free(outBuf);
    // API sequence test completed successfully
    return 66;
}