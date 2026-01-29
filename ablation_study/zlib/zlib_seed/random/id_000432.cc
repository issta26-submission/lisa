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
//<ID> 432
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compute compile flags
    const char src[] = "zlib API sequence payload: compress -> inflateInit2_ -> inflate -> gzprintf -> inflateEnd -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong flags = zlibCompileFlags();

    // step 2: Compress the source into a buffer
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);

    // step 3: Initialize inflate with inflateInit2_, perform inflate, then call inflateEnd
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init2 = inflateInit2_(&strm, 15, version, (int)sizeof(z_stream));
    strm.next_in = comp;
    strm.avail_in = (uInt)compLen;
    Bytef *out = (Bytef *)malloc((size_t)sourceLen);
    memset(out, 0, (size_t)sourceLen);
    strm.next_out = out;
    strm.avail_out = (uInt)sourceLen;
    int rc_inflate = inflate(&strm, 0);
    uLong inflated_total = strm.total_out;
    int rc_inf_end = inflateEnd(&strm);

    // step 4: Write a small report using gzprintf, then cleanup
    gzFile gzf = gzopen("zlib_api_sequence_tmp.gz", "wb");
    gzprintf(gzf, "zlib flags=%lu src_len=%lu inflated=%lu comp_len=%lu\n", flags, (uLong)sourceLen, inflated_total, (uLong)compLen);
    gzclose(gzf);
    free(comp);
    free(out);
    (void)rc_comp;
    (void)rc_inf_init2;
    (void)rc_inflate;
    (void)rc_inf_end;
    (void)version;
    (void)flags;
    // API sequence test completed successfully
    return 66;
}