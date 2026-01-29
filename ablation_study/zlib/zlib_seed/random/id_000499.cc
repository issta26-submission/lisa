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
//<ID> 499
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress it with compress2
    const char src[] = "zlib API sequence payload: prepare -> compress2 -> deflateBound -> gzprintf -> inflate -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong compBound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)compBound);
    memset(comp, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(comp, &compLen, source, sourceLen, 6);

    // step 2: Initialize a deflate stream and compute an output bound using deflateBound
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong outBound = deflateBound(&dstrm, (uLong)compLen);
    Bytef *outBuf = (Bytef *)malloc((size_t)outBound);
    memset(outBuf, 0, (size_t)outBound);

    // step 3: Open a gzFile, write a human-readable header with gzprintf, and check direct I/O
    gzFile gf = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gzprint = gzprintf(gf, "%s", src);
    int direct = gzdirect(gf);

    // step 4: Initialize inflate, decompress the compressed buffer into outBuf, then cleanup everything
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outBound;
    int rc_inflate = inflate(&istrm, 0);
    int rc_inf_end = inflateEnd(&istrm);
    int rc_def_end = deflateEnd(&dstrm);
    int rc_gzclose = gzclose(gf);
    free(comp);
    free(outBuf);
    (void)rc_comp;
    (void)rc_def_init;
    (void)rc_gzprint;
    (void)direct;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_inf_end;
    (void)rc_def_end;
    (void)rc_gzclose;
    (void)version;
    // API sequence test completed successfully
    return 66;
}