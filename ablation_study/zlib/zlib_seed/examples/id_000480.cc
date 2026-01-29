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
//<ID> 480
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compute initial adler32
    const char src[] = "zlib API sequence payload for deflate -> gz write -> crc combine";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong ad1 = adler32(0L, source, (uInt)sourceLen);

    // step 2: Initialize and run a deflate stream, obtain output buffer using deflateBound
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    uLong outBound = deflateBound(&dstrm, sourceLen);
    Bytef *outBuf = (Bytef *)malloc((size_t)outBound);
    memset(outBuf, 0, (size_t)outBound);
    dstrm.next_out = outBuf;
    dstrm.avail_out = (uInt)outBound;
    int rc_def = deflate(&dstrm, 0);

    // step 3: Write deflate output to a gzFile, query direct I/O and 64-bit offset, compute combined adler32
    gzFile gf = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gzwrite = gzwrite(gf, (voidpc)outBuf, (unsigned int)dstrm.total_out);
    int direct = gzdirect(gf);
    off64_t off64 = gzoffset64(gf);
    uLong ad2 = adler32(0L, outBuf, (uInt)dstrm.total_out);
    uLong combined = adler32_combine(ad1, ad2, (off_t)sourceLen);
    int rc_gzclose = gzclose(gf);

    // step 4: Cleanup deflate stream and memory
    int rc_def_end = deflateEnd(&dstrm);
    free(outBuf);
    (void)rc_def_init;
    (void)rc_def;
    (void)rc_gzwrite;
    (void)direct;
    (void)off64;
    (void)combined;
    (void)rc_gzclose;
    (void)rc_def_end;
    (void)version;
    // API sequence test completed successfully
    return 66;
}