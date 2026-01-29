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
//<ID> 373
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and one-shot compression of a payload
    const char src[] = "zlib API sequence payload: compress2 -> deflateInit_ -> uncompress2 -> gzwrite -> gztell -> gzclose";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong bound1 = compressBound(sourceLen);
    uLongf compLen1 = (uLongf)bound1;
    Bytef *comp1 = (Bytef *)malloc((size_t)bound1);
    memset(comp1, 0, (size_t)bound1);
    int rc_comp2 = compress2(comp1, &compLen1, source, sourceLen, 6);

    // step 2: Initialize a deflate stream, perform a deflate pass on the compressed blob, finalize
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = comp1;
    dstrm.avail_in = (uInt)compLen1;
    uLong bound2 = deflateBound(&dstrm, (uLong)compLen1);
    Bytef *comp2 = (Bytef *)malloc((size_t)bound2);
    memset(comp2, 0, (size_t)bound2);
    dstrm.next_out = comp2;
    dstrm.avail_out = (uInt)bound2;
    int rc_deflate = deflate(&dstrm, 0);
    uLong out2 = dstrm.total_out;
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Decompress the one-shot compressed buffer using uncompress2 into a destination buffer
    uLongf destLen = (uLongf)sourceLen;
    Bytef *destBuf = (Bytef *)malloc((size_t)sourceLen);
    memset(destBuf, 0, (size_t)sourceLen);
    uLong srcLenForUncomp = (uLong)compLen1;
    int rc_uncompress2 = uncompress2(destBuf, &destLen, comp1, &srcLenForUncomp);

    // step 4: Write compressed data to a gzip file, query position with gztell, close and cleanup
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile gzf = gzopen(fname, "wb");
    int rc_gzwrite1 = gzwrite(gzf, comp1, (unsigned int)compLen1);
    int rc_gzwrite2 = gzwrite(gzf, comp2, (unsigned int)out2);
    off_t pos = gztell(gzf);
    int rc_gzclose = gzclose(gzf);
    free(comp1);
    free(comp2);
    free(destBuf);

    (void)version;
    (void)rc_comp2;
    (void)rc_def_init;
    (void)rc_deflate;
    (void)rc_def_end;
    (void)out2;
    (void)destLen;
    (void)rc_uncompress2;
    (void)srcLenForUncomp;
    (void)rc_gzwrite1;
    (void)rc_gzwrite2;
    (void)pos;
    (void)rc_gzclose;
    (void)bound1;
    (void)bound2;
    (void)sourceLen;

    // API sequence test completed successfully
    return 66;
}