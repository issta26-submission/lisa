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
//<ID> 475
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and obtain compressed data via compress()
    const char src[] = "zlib API sequence payload: prepare -> compress -> deflatePending -> inflate -> gz IO -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);

    // step 2: Initialize inflate and deflate streams
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    inf_strm.next_in = comp;
    inf_strm.avail_in = (uInt)compLen;
    uLong outCap = sourceLen * 2 + 16;
    Bytef *dest = (Bytef *)malloc((size_t)outCap);
    memset(dest, 0, (size_t)outCap);
    inf_strm.next_out = dest;
    inf_strm.avail_out = (uInt)outCap;

    z_stream def_strm;
    memset(&def_strm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)sourceLen;
    uLong defOutCap = deflateBound(&def_strm, sourceLen);
    Bytef *defOut = (Bytef *)malloc((size_t)defOutCap);
    memset(defOut, 0, (size_t)defOutCap);
    def_strm.next_out = defOut;
    def_strm.avail_out = (uInt)defOutCap;

    // step 3: Operate streams and inspect pending bits, then inflate compressed buffer
    int rc_def_op = deflate(&def_strm, 0);
    unsigned int pending = 0;
    int bits = 0;
    int rc_pending = deflatePending(&def_strm, &pending, &bits);
    int rc_inf = inflate(&inf_strm, 0);
    int rc_inf_end = inflateEnd(&inf_strm);

    // step 4: Write deflate output to gzFile, query direct mode, close and cleanup
    gzFile gf = gzopen("zlib_api_sequence_out.gz", "wb");
    int is_direct = gzdirect(gf);
    z_size_t written = gzfwrite((voidpc)defOut, (z_size_t)1, (z_size_t)def_strm.total_out, gf);
    int rc_gzclose = gzclose(gf);
    int rc_def_end = deflateEnd(&def_strm);
    free(comp);
    free(dest);
    free(defOut);
    (void)rc_comp;
    (void)rc_inf_init;
    (void)rc_def_init;
    (void)rc_def_op;
    (void)rc_pending;
    (void)rc_inf;
    (void)rc_inf_end;
    (void)is_direct;
    (void)written;
    (void)rc_gzclose;
    (void)rc_def_end;
    (void)version;
    // API sequence test completed successfully
    return 66;
}