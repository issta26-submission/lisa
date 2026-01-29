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
//<ID> 580
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compute checksums
    const char src[] = "zlib API sequence: prepare -> deflateInit_ -> inflateGetHeader -> gz write";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong adler = adler32(0L, source, (uInt)sourceLen);
    uLong crc_part1 = crc32(0L, source, (uInt)(sourceLen / 2));
    uLong crc_part2 = crc32(0L, source + (sourceLen / 2), (uInt)(sourceLen - (sourceLen / 2)));
    uLong crc_combined = crc32_combine(crc_part1, crc_part2, (off_t)(sourceLen - (sourceLen / 2)));

    // step 2: Initialize and perform a deflate operation
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong outBound = deflateBound(&dstrm, sourceLen);
    Bytef *outBuf = (Bytef *)malloc((size_t)outBound);
    memset(outBuf, 0, (size_t)outBound);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = outBuf;
    dstrm.avail_out = (uInt)outBound;
    int rc_def = deflate(&dstrm, 4); /* Z_FINISH == 4 */

    // step 3: Initialize an inflate stream and query gzip header info
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, sizeof(gz_header));
    int rc_get_head = inflateGetHeader(&istrm, &head);
    int rc_inf_end = inflateEnd(&istrm);

    // step 4: Write compressed output to a gzFile, query direct mode, and cleanup
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, outBuf, (unsigned int)dstrm.total_out);
    int is_direct = gzdirect(gf);
    int rc_gzclose = gzclose(gf);
    int rc_def_end = deflateEnd(&dstrm);
    free(outBuf);
    (void)adler;
    (void)crc_combined;
    (void)rc_def_init;
    (void)rc_def;
    (void)rc_inf_init;
    (void)rc_get_head;
    (void)is_direct;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_def_end;
    // API sequence test completed successfully
    return 66;
}