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
//<ID> 360
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and prepare payload/version
    const char payload[] = "zlib API sequence payload: compress2 -> crc32_z -> deflate -> gzwrite -> gzclose";
    const Bytef *source = (const Bytef *)payload;
    uLong sourceLen = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();

    // step 2: Compress with compress2 and compute CRCs
    uLong bound_comp = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound_comp);
    memset(compBuf, 0, (size_t)bound_comp);
    uLongf compLen = (uLongf)bound_comp;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);
    uLong crc_source = crc32_z(0UL, source, (z_size_t)sourceLen);
    uLong crc_comp = crc32_z(0UL, compBuf, (z_size_t)compLen);

    // step 3: Initialize deflate stream, compute bound, perform deflate, and finish
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong bound_def = deflateBound(&dstrm, sourceLen);
    Bytef *defBuf = (Bytef *)malloc((size_t)bound_def);
    memset(defBuf, 0, (size_t)bound_def);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = defBuf;
    dstrm.avail_out = (uInt)bound_def;
    int rc_def = deflate(&dstrm, Z_FINISH);
    int rc_def_end = deflateEnd(&dstrm);

    // step 4: Write compressed data to a gzip file and cleanup
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile gzf = gzopen(fname, "wb");
    int rc_gzwrite1 = gzwrite(gzf, (voidpc)compBuf, (unsigned int)compLen);
    int rc_gzwrite2 = gzwrite(gzf, (voidpc)defBuf, (unsigned int)dstrm.total_out);
    int rc_gzclose = gzclose(gzf);
    free(compBuf);
    free(defBuf);

    (void)version;
    (void)rc_comp;
    (void)crc_source;
    (void)crc_comp;
    (void)rc_def_init;
    (void)rc_def;
    (void)rc_def_end;
    (void)rc_gzwrite1;
    (void)rc_gzwrite2;
    (void)rc_gzclose;

    // API sequence test completed successfully
    return 66;
}