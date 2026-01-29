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
//<ID> 702
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and version
    const char src[] = "zlib API sequence test payload for inflateBackInit_, deflateBound, inflateEnd and gzclose";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();

    // step 2: Initialize a deflate template, compute bound, compress, then clean up deflate stream
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong compAlloc = deflateBound(&dstrm, sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf compLen = (uLongf)compAlloc;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Initialize inflateBack stream (window buffer) and a regular inflate stream to decompress the compressed buffer
    z_stream backstrm;
    memset(&backstrm, 0, sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int rc_back_init = inflateBackInit_(&backstrm, 15, window, version, (int)sizeof(z_stream));

    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    uLong outAlloc = sourceLen + 64;
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outAlloc;
    int rc_inflate = inflate(&istrm, 0);
    uLong decompLen = istrm.total_out;
    int rc_inf_end = inflateEnd(&istrm);
    int rc_back_end = inflateBackEnd(&backstrm);

    // step 4: Write decompressed data to a gz file, close it, free resources and finish
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, outBuf, (unsigned int)decompLen);
    int rc_gzclose = gzclose(gf);
    free(compBuf);
    free(outBuf);
    free(window);
    (void)rc_def_init;
    (void)rc_comp;
    (void)rc_def_end;
    (void)rc_back_init;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_inf_end;
    (void)rc_back_end;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}