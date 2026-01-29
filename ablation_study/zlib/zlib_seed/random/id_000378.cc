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
//<ID> 378
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and prepare payloads and buffers
    const char src[] = "zlib API sequence payload: compress2 -> uncompress2 -> deflate -> gzwrite -> gztell -> gzclose";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong bound_comp1 = compressBound(sourceLen);
    uLongf comp1Len_f = (uLongf)bound_comp1;
    Bytef *comp1 = (Bytef *)malloc((size_t)bound_comp1);
    memset(comp1, 0, (size_t)bound_comp1);
    int rc_comp2 = compress2(comp1, &comp1Len_f, source, sourceLen, 6);
    uLong comp1Len = (uLong)comp1Len_f;

    // step 2: Decompress the compressed blob using uncompress2
    uLongf destLen_f = (uLongf)sourceLen;
    Bytef *decomp = (Bytef *)malloc((size_t)sourceLen);
    memset(decomp, 0, (size_t)sourceLen);
    uLong srcLen_for_uncomp = comp1Len;
    int rc_uncomp2 = uncompress2(decomp, &destLen_f, comp1, &srcLen_for_uncomp);
    uLong decomp_out_len = (uLong)destLen_f;

    // step 3: Initialize a deflate stream and run deflate on the decompressed data
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    strm.next_in = decomp;
    strm.avail_in = (uInt)decomp_out_len;
    uLong bound_comp2 = deflateBound(&strm, decomp_out_len);
    Bytef *comp2 = (Bytef *)malloc((size_t)bound_comp2);
    memset(comp2, 0, (size_t)bound_comp2);
    strm.next_out = comp2;
    strm.avail_out = (uInt)bound_comp2;
    int rc_deflate = deflate(&strm, 0);
    uLong comp2_out = strm.total_out;
    int rc_def_end = deflateEnd(&strm);

    // step 4: Write result to a gzip file, query position with gztell, then close and cleanup
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile gzf = gzopen(fname, "wb");
    int rc_gzwrite = gzwrite(gzf, comp2, (unsigned int)comp2_out);
    off_t pos = gztell(gzf);
    int rc_gzclose = gzclose(gzf);
    free(comp1);
    free(decomp);
    free(comp2);

    (void)version;
    (void)rc_comp2;
    (void)rc_uncomp2;
    (void)rc_def_init;
    (void)rc_deflate;
    (void)rc_def_end;
    (void)rc_gzwrite;
    (void)pos;
    (void)rc_gzclose;
    (void)comp1Len;
    (void)comp2_out;
    (void)bound_comp1;
    (void)bound_comp2;

    // API sequence test completed successfully
    return 66;
}