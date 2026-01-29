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
//<ID> 583
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compute an initial adler32 checksum
    const char src[] = "zlib API sequence: prepare -> deflate -> inflateGetHeader -> gz write -> crc combine";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong adler_initial = adler32(0UL, source, (uInt)sourceLen);
    uInt outBufSize = (uInt)(sourceLen + 256);
    Bytef *outBuf = (Bytef *)malloc((size_t)outBufSize);
    memset(outBuf, 0, (size_t)outBufSize);

    // step 2: Initialize a deflate stream, compress the source into outBuf, then end deflate
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = outBuf;
    dstrm.avail_out = outBufSize;
    int rc_def = deflate(&dstrm, 4); /* Z_FINISH == 4 */
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Initialize an inflate stream and query its gzip header structure
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    gz_header header;
    memset(&header, 0, sizeof(gz_header));
    int rc_gethdr = inflateGetHeader(&istrm, &header);
    int rc_inf_end = inflateEnd(&istrm);

    // step 4: Write compressed output to a gzFile, query direct mode, combine CRCs, and cleanup
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, outBuf, (unsigned int)dstrm.total_out);
    int is_direct = gzdirect(gf);
    uLong crc_combo = crc32_combine(adler_initial, (uLong)dstrm.adler, (off_t)dstrm.total_out);
    int rc_gzclose = gzclose(gf);
    free(outBuf);
    (void)rc_def_init;
    (void)rc_def;
    (void)rc_def_end;
    (void)rc_inf_init;
    (void)rc_gethdr;
    (void)rc_inf_end;
    (void)rc_gzwrite;
    (void)is_direct;
    (void)crc_combo;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}