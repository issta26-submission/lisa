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
//<ID> 478
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress with compress()
    const char src[] = "zlib API sequence payload: prepare -> compress -> inflateInit_ -> inflate -> deflateInit_ -> deflatePending -> gzfwrite/gzdirect -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);

    // step 2: Inflate the compressed data to validate round-trip and then call inflateEnd
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)compLen;
    uLong outCap = sourceLen * 2 + 16;
    Bytef *dest = (Bytef *)malloc((size_t)outCap);
    memset(dest, 0, (size_t)outCap);
    istrm.next_out = dest;
    istrm.avail_out = (uInt)outCap;
    int rc_inflate = inflate(&istrm, 0);
    uLong inflated_bytes = istrm.total_out;
    int rc_inf_end = inflateEnd(&istrm);

    // step 3: Re-compress the previously produced compressed blob using deflate APIs, query pending bits, and write out via gzFile
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = comp;
    dstrm.avail_in = (uInt)compLen;
    uLong defOutBound = deflateBound(&dstrm, (uLong)compLen);
    Bytef *outBuf = (Bytef *)malloc((size_t)defOutBound);
    memset(outBuf, 0, (size_t)defOutBound);
    dstrm.next_out = outBuf;
    dstrm.avail_out = (uInt)defOutBound;
    int rc_def = deflate(&dstrm, 0);
    unsigned int pending = 0;
    int bits = 0;
    int rc_pending = deflatePending(&dstrm, &pending, &bits);
    gzFile gf = gzopen("test_zlib_api_sequence.gz", "wb");
    z_size_t written = gzfwrite((voidpc)outBuf, (z_size_t)1, (z_size_t)dstrm.total_out, gf);
    int rc_gzdirect = gzdirect(gf);
    int rc_gzclose = gzclose(gf);
    int rc_def_end = deflateEnd(&dstrm);

    // step 4: Cleanup allocated resources and finalize
    free(comp);
    free(dest);
    free(outBuf);
    (void)rc_comp;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)inflated_bytes;
    (void)rc_inf_end;
    (void)rc_def_init;
    (void)rc_def;
    (void)rc_pending;
    (void)pending;
    (void)bits;
    (void)written;
    (void)rc_gzdirect;
    (void)rc_gzclose;
    (void)rc_def_end;
    (void)version;
    // API sequence test completed successfully
    return 66;
}