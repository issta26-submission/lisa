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
//<ID> 584
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compute checksums
    const char src[] = "zlib test payload: deflate -> inflate -> header -> gz operations";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong adler = adler32(0L, source, (uInt)sourceLen);
    uLong crc_first = crc32(0L, source, (uInt)(sourceLen / 2));
    uLong crc_second = crc32(0L, source + (sourceLen / 2), (uInt)(sourceLen - (sourceLen / 2)));
    uLong crc_combined = crc32_combine(crc_first, crc_second, (off_t)(sourceLen - (sourceLen / 2)));

    // step 2: Initialize deflate stream, compress the source into a buffer
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong outBound = deflateBound(&dstrm, sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)outBound);
    memset(compBuf, 0, (size_t)outBound);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)outBound;
    int rc_def = deflate(&dstrm, 4); /* Z_FINISH == 4 */
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Initialize inflate stream, attempt to inflate and query gzip header
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)dstrm.total_out;
    uLong decompBound = sourceLen + 64;
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompBound);
    memset(decompBuf, 0, (size_t)decompBound);
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)decompBound;
    int rc_inflate = inflate(&istrm, 4); /* Z_FINISH == 4 */
    gz_header head;
    memset(&head, 0, sizeof(gz_header));
    int rc_get_header = inflateGetHeader(&istrm, &head);
    int rc_inf_end = inflateEnd(&istrm);

    // step 4: Write original data to a gzFile, query direct mode, and cleanup
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, source, (unsigned int)sourceLen);
    int rc_gzdirect = gzdirect(gf);
    int rc_gzclose = gzclose(gf);
    free(compBuf);
    free(decompBuf);

    (void)adler;
    (void)crc_first;
    (void)crc_second;
    (void)crc_combined;
    (void)rc_def_init;
    (void)rc_def;
    (void)rc_def_end;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_get_header;
    (void)rc_inf_end;
    (void)rc_gzwrite;
    (void)rc_gzdirect;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}