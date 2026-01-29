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
//<ID> 367
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and prepare payload and version
    const char payload[] = "zlib API sequence payload: compress2 -> crc32_z -> gzwrite -> deflate usage";
    const Bytef *source = (const Bytef *)payload;
    uLong sourceLen = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;

    // step 2: One-shot compression, compute CRC of compressed data, write to gzip and close
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);
    uLong crc_comp = crc32_z(0UL, compBuf, (z_size_t)compLen);
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile gzf = gzopen(fname, "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)compBuf, (unsigned int)compLen);
    int rc_gzclose = gzclose(gzf);

    // step 3: Initialize a deflate stream and perform a single deflate call on the compressed buffer
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    uLong defOutBound = compressBound((uLong)compLen);
    Bytef *defOut = (Bytef *)malloc((size_t)defOutBound);
    memset(defOut, 0, (size_t)defOutBound);
    strm.next_in = compBuf;
    strm.avail_in = (uInt)compLen;
    strm.next_out = defOut;
    strm.avail_out = (uInt)defOutBound;
    int rc_def = deflate(&strm, 0);
    int rc_def_end = deflateEnd(&strm);

    // step 4: Cleanup
    free(compBuf);
    free(defOut);
    (void)version;
    (void)rc_comp;
    (void)crc_comp;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_def_init;
    (void)rc_def;
    (void)rc_def_end;
    // API sequence test completed successfully
    return 66;
}