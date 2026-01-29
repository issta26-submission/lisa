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
//<ID> 379
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare payload, compress it one-shot, and then uncompress with uncompress2
    const char src[] = "zlib API sequence payload: compress2 -> uncompress2 -> deflateInit_ -> gzwrite -> gztell -> gzclose";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong bound1 = compressBound(sourceLen);
    uLongf comp1Len = (uLongf)bound1;
    Bytef *comp1 = (Bytef *)malloc((size_t)bound1);
    memset(comp1, 0, (size_t)bound1);
    int rc_comp2 = compress2(comp1, &comp1Len, source, sourceLen, 6);
    Bytef *roundtrip = (Bytef *)malloc((size_t)sourceLen);
    memset(roundtrip, 0, (size_t)sourceLen);
    uLongf roundtripLen = (uLongf)sourceLen;
    uLong comp1SrcLen = (uLong)comp1Len;
    int rc_uncomp2 = uncompress2(roundtrip, &roundtripLen, comp1, &comp1SrcLen);

    // step 2: Initialize a deflate stream and perform streaming deflate
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)sourceLen;
    uLong bound2 = deflateBound(&strm, sourceLen);
    Bytef *comp2 = (Bytef *)malloc((size_t)bound2);
    memset(comp2, 0, (size_t)bound2);
    strm.next_out = comp2;
    strm.avail_out = (uInt)bound2;
    int rc_deflate = deflate(&strm, 0);
    uLong out2 = strm.total_out;
    int rc_def_end = deflateEnd(&strm);

    // step 3: Write deflated data to a gz file, query position with gztell, then close
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile gzf = gzopen(fname, "wb");
    int rc_gzwrite = gzwrite(gzf, comp2, (unsigned int)out2);
    off_t pos = gztell(gzf);
    int rc_gzclose = gzclose(gzf);

    // step 4: Cleanup buffers and silence unused warnings
    free(comp1);
    free(roundtrip);
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
    // API sequence test completed successfully
    return 66;
}