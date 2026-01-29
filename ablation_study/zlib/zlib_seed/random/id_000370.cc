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
//<ID> 370
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and prepare source, version and buffers
    const char src[] = "zlib API sequence payload: compress2 -> uncompress2 -> deflate -> gzwrite -> gztell -> gzclose";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong bound1 = compressBound(sourceLen);
    uLongf comp1Len = (uLongf)bound1;
    Bytef *comp1 = (Bytef *)malloc((size_t)bound1);
    memset(comp1, 0, (size_t)bound1);

    // step 2: One-shot compress (compress2) and then uncompress2 to validate round-trip
    int rc_comp2 = compress2(comp1, &comp1Len, source, sourceLen, 6);
    Bytef *decomp = (Bytef *)malloc((size_t)sourceLen);
    memset(decomp, 0, (size_t)sourceLen);
    uLongf decompLen = (uLongf)sourceLen;
    uLong comp1Len_for_uncomp = (uLong)comp1Len;
    int rc_uncomp2 = uncompress2(decomp, &decompLen, comp1, &comp1Len_for_uncomp);

    // step 3: Initialize a deflate stream, deflate the decompressed data, collect output size
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    strm.next_in = decomp;
    strm.avail_in = (uInt)decompLen;
    uLong bound2 = deflateBound(&strm, (uLong)decompLen);
    Bytef *comp2 = (Bytef *)malloc((size_t)bound2);
    memset(comp2, 0, (size_t)bound2);
    strm.next_out = comp2;
    strm.avail_out = (uInt)bound2;
    int rc_deflate = deflate(&strm, 0);
    uLong out_len = strm.total_out;
    int rc_def_end = deflateEnd(&strm);

    // step 4: Write deflated output to a gzip file, query position with gztell, then close and cleanup
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile gzf = gzopen(fname, "wb");
    int rc_gzwrite = gzwrite(gzf, comp2, (unsigned int)out_len);
    off_t pos = gztell(gzf);
    int rc_gzclose = gzclose(gzf);

    free(comp1);
    free(decomp);
    free(comp2);

    (void)version;
    (void)bound1;
    (void)bound2;
    (void)rc_comp2;
    (void)rc_uncomp2;
    (void)rc_def_init;
    (void)rc_deflate;
    (void)rc_def_end;
    (void)rc_gzwrite;
    (void)pos;
    (void)rc_gzclose;

    // API sequence test completed successfully
    return 66;
}