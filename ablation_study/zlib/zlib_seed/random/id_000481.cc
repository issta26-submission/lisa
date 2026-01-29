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
//<ID> 481
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data, compute adler32 pieces and combine checksums
    const char src[] = "zlib API sequence payload: initialize -> deflateBound -> deflate -> gz I/O -> inflate -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char tail_str[] = "tail";
    const Bytef *tail = (const Bytef *)tail_str;
    uLong tailLen = (uLong)(sizeof(tail_str) - 1);
    uLong a1 = adler32(0UL, source, (uInt)sourceLen);
    uLong a2 = adler32(0UL, tail, (uInt)tailLen);
    uLong combined_adler = adler32_combine(a1, a2, (off_t)tailLen);

    // step 2: Initialize and configure a deflate stream, obtain deflateBound and allocate output buffer
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    uLong outBound = deflateBound(&dstrm, (uLong)sourceLen);
    Bytef *outBuf = (Bytef *)malloc((size_t)outBound);
    memset(outBuf, 0, (size_t)outBound);
    dstrm.next_out = outBuf;
    dstrm.avail_out = (uInt)outBound;

    // step 3: Operate deflate, write to gzFile, query direct I/O and 64-bit offset
    int rc_def_op = deflate(&dstrm, 0);
    gzFile gf = gzopen("test_zlib_api_sequence_output.gz", "wb");
    int rc_gzwrite = gzwrite(gf, (voidpc)outBuf, (unsigned int)dstrm.total_out);
    int direct = gzdirect(gf);
    off64_t offset64 = gzoffset64(gf);
    int rc_gzclose = gzclose(gf);

    // step 4: Initialize inflate, decompress the deflate output, cleanup streams and memory
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = outBuf;
    istrm.avail_in = (uInt)dstrm.total_out;
    uLong destBound = sourceLen * 2 + 32;
    Bytef *dest = (Bytef *)malloc((size_t)destBound);
    memset(dest, 0, (size_t)destBound);
    istrm.next_out = dest;
    istrm.avail_out = (uInt)destBound;
    int rc_inflate = inflate(&istrm, 0);
    int rc_inf_end = inflateEnd(&istrm);
    int rc_def_end = deflateEnd(&dstrm);
    free(outBuf);
    free(dest);
    (void)combined_adler;
    (void)rc_def_init;
    (void)rc_def_op;
    (void)rc_gzwrite;
    (void)direct;
    (void)offset64;
    (void)rc_gzclose;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_inf_end;
    (void)rc_def_end;
    (void)version;
    // API sequence test completed successfully
    return 66;
}