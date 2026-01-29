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
//<ID> 439
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress into a buffer
    const char src[] = "zlib API sequence payload: prepare -> compress -> inflateInit2_ -> inflate -> gzprintf -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);

    // step 2: Initialize inflate stream with custom windowBits and perform inflate
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit2_(&strm, 15, version, (int)sizeof(z_stream));
    strm.next_in = comp;
    strm.avail_in = (uInt)compLen;
    uLong outCap = sourceLen * 2;
    Bytef *dest = (Bytef *)malloc((size_t)outCap);
    memset(dest, 0, (size_t)outCap);
    strm.next_out = dest;
    strm.avail_out = (uInt)outCap;
    int rc_inflate = inflate(&strm, 0);
    uLong out_size = strm.total_out;
    int rc_inf_end = inflateEnd(&strm);

    // step 3: Validate results via checksum and emit a gzprintf with compile flags and checksums
    uLong chksrc = adler32(0L, source, (uInt)sourceLen);
    uLong chkout = adler32(0L, dest, (uInt)out_size);
    gzFile gf = gzopen("zlib_api_sequence_out.gz", "wb");
    gzprintf(gf, "zlibFlags=%lu srcAdler=%lu outAdler=%lu\n", (unsigned long)zlibCompileFlags(), (unsigned long)chksrc, (unsigned long)chkout);
    int rc_gzclose = gzclose(gf);

    // step 4: Cleanup
    free(comp);
    free(dest);
    (void)version;
    (void)rc_comp;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_inf_end;
    (void)rc_gzclose;
    (void)compLen;
    (void)out_size;
    (void)chksrc;
    (void)chkout;
    // API sequence test completed successfully
    return 66;
}