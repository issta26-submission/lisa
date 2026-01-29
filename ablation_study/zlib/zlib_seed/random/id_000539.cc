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
//<ID> 539
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data, query zlib version, allocate buffers and produce a single-shot compressed buffer with compress2
    const char src[] = "zlib API sequence payload: prepare -> compress2 -> deflate stream -> inflate -> gz offset -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong compBound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)compBound);
    memset(comp, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(comp, &compLen, source, sourceLen, 6);

    // step 2: Initialize a deflate stream, perform a one-shot stream deflate of the same source, and finalize the deflate stream
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong dbound = deflateBound(&dstrm, sourceLen);
    Bytef *dcomp = (Bytef *)malloc((size_t)dbound);
    memset(dcomp, 0, (size_t)dbound);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = dcomp;
    dstrm.avail_out = (uInt)dbound;
    int rc_deflate = deflate(&dstrm, 4); /* Z_FINISH == 4 */
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Initialize an inflate stream with inflateInit_, decompress the compress2 output into a destination buffer, and finalize inflate
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    uLongf destLen = (uLongf)(sourceLen + 128);
    Bytef *dest = (Bytef *)malloc((size_t)destLen);
    memset(dest, 0, (size_t)destLen);
    istrm.next_in = comp;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = dest;
    istrm.avail_out = (uInt)destLen;
    int rc_inflate = inflate(&istrm, 0); /* Z_NO_FLUSH == 0 */
    int rc_inf_end = inflateEnd(&istrm);

    // step 4: Write original data to a gzFile, obtain 64-bit offset with gzoffset64, close file, cleanup buffers and return
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, source, (unsigned int)sourceLen);
    off64_t off64 = gzoffset64(gf);
    int rc_gzclose = gzclose(gf);
    free(comp);
    free(dcomp);
    free(dest);
    (void)version;
    (void)rc_comp;
    (void)rc_def_init;
    (void)rc_deflate;
    (void)rc_def_end;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_inf_end;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)off64;
    // API sequence test completed successfully
    return 66;
}