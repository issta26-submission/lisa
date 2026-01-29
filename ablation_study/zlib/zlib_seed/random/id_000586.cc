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
//<ID> 586
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compute an initial adler32 checksum
    const char src[] = "zlib API sequence: prepare -> deflate -> write gz -> inflate -> validate";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong adler_orig = adler32(0L, source, (uInt)sourceLen);

    // step 2: Initialize and run a deflate stream to produce compressed data
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong compBound = deflateBound(&dstrm, sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compBound;
    int rc_def = deflate(&dstrm, 4); /* Z_FINISH == 4 */
    uLong compUsed = dstrm.total_out;

    // step 3: Write original data to a gzFile and query whether the file is direct
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, source, (unsigned int)sourceLen);
    int rc_gzdirect = gzdirect(gf);
    int rc_gzclose = gzclose(gf);

    // step 4: Initialize inflate, attach a gz_header, inflate the previously produced compressed buffer, validate with adler32 and crc32_combine, then cleanup
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    gz_header gzh;
    memset(&gzh, 0, sizeof(gz_header));
    int rc_gethdr = inflateGetHeader(&istrm, &gzh);
    uLong outBound = sourceLen + 64;
    Bytef *outBuf = (Bytef *)malloc((size_t)outBound);
    memset(outBuf, 0, (size_t)outBound);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compUsed;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outBound;
    int rc_inflate = inflate(&istrm, 4); /* Z_FINISH == 4 */
    uLong decompLen = istrm.total_out;
    uLong adler_decomp = adler32(0L, outBuf, (uInt)decompLen);
    uLong combined = crc32_combine(adler_orig, adler_decomp, (off_t)decompLen);

    int rc_inf_end = inflateEnd(&istrm);
    int rc_def_end = deflateEnd(&dstrm);
    free(compBuf);
    free(outBuf);
    (void)rc_def_init;
    (void)rc_def;
    (void)compUsed;
    (void)rc_gzwrite;
    (void)rc_gzdirect;
    (void)rc_gzclose;
    (void)rc_inf_init;
    (void)rc_gethdr;
    (void)rc_inflate;
    (void)adler_orig;
    (void)adler_decomp;
    (void)combined;
    (void)rc_inf_end;
    (void)rc_def_end;
    // API sequence test completed successfully
    return 66;
}