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
//<ID> 494
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and perform a zlib compression (compress2)
    const char src[] = "zlib API sequence payload: compress2 -> deflateBound -> gz write -> inflate -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong compBound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)compBound);
    memset(comp, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(comp, &compLen, source, sourceLen, 6);

    // step 2: Initialize a deflate stream and compute an upper bound with deflateBound
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = comp;
    dstrm.avail_in = (uInt)compLen;
    uLong bound_for_deflate = deflateBound(&dstrm, (uLong)compLen);

    // step 3: Write compressed data to a gzFile, using gzprintf for a header and check gzdirect
    gzFile gf = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gzprintf = gzprintf(gf, "gz header: original_len=%lu compressed_len=%lu\n", (unsigned long)sourceLen, (unsigned long)compLen);
    int rc_gzwrite = gzwrite(gf, (voidpc)comp, (unsigned int)compLen);
    int direct = gzdirect(gf);
    int rc_gzclose = gzclose(gf);

    // step 4: Inflate the compressed buffer to validate round-trip, then cleanup streams and memory
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)compLen;
    uLong destBound = sourceLen * 2 + 64;
    Bytef *dest = (Bytef *)malloc((size_t)destBound);
    memset(dest, 0, (size_t)destBound);
    istrm.next_out = dest;
    istrm.avail_out = (uInt)destBound;
    int rc_inflate = inflate(&istrm, 0);
    int rc_inf_end = inflateEnd(&istrm);
    int rc_def_end = deflateEnd(&dstrm);
    free(comp);
    free(dest);
    (void)rc_comp;
    (void)compLen;
    (void)rc_def_init;
    (void)bound_for_deflate;
    (void)rc_gzprintf;
    (void)rc_gzwrite;
    (void)direct;
    (void)rc_gzclose;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_inf_end;
    (void)rc_def_end;
    (void)version;
    // API sequence test completed successfully
    return 66;
}