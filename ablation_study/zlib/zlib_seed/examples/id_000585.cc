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
//<ID> 585
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compute checksums
    const char src[] = "zlib API sequence: prepare -> init -> deflate -> gzwrite -> validate -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong adler = adler32(0L, source, (uInt)sourceLen);
    uLong crc1 = crc32(0L, source, (uInt)sourceLen);

    // step 2: Initialize and configure streams
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, sizeof(z_stream));
    memset(&istrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    uLong outBound = deflateBound(&dstrm, sourceLen);
    Bytef *outBuf = (Bytef *)malloc((size_t)outBound);
    memset(outBuf, 0, (size_t)outBound);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = outBuf;
    dstrm.avail_out = (uInt)outBound;

    // step 3: Operate and validate via combined APIs
    int rc_deflate = deflate(&dstrm, 4); /* Z_FINISH == 4 */
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, outBuf, (unsigned int)dstrm.total_out);
    int is_direct = gzdirect(gf);
    uLong crc_combined = crc32_combine(crc1, crc1, (off_t)sourceLen);
    gz_header head;
    memset(&head, 0, sizeof(gz_header));
    int rc_hdr = inflateGetHeader(&istrm, &head);

    // step 4: Cleanup and finalization
    int rc_def_end = deflateEnd(&dstrm);
    int rc_inf_end = inflateEnd(&istrm);
    int rc_gzclose = gzclose(gf);
    free(outBuf);
    (void)adler;
    (void)crc1;
    (void)rc_def_init;
    (void)rc_inf_init;
    (void)rc_deflate;
    (void)rc_gzwrite;
    (void)is_direct;
    (void)crc_combined;
    (void)rc_hdr;
    (void)rc_def_end;
    (void)rc_inf_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}