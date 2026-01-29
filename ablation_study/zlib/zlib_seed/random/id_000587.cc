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
//<ID> 587
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compute checksums
    const char src[] = "zlib API sequence payload: prepare -> deflate -> inflate -> gzwrite";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong adler_src = adler32(1UL, source, (uInt)sourceLen);
    uInt len1 = (uInt)(sourceLen / 2);
    uInt len2 = (uInt)(sourceLen - len1);
    uLong crc_part1 = crc32(0UL, source, len1);
    uLong crc_part2 = crc32(0UL, source + len1, len2);
    uLong crc_combined = crc32_combine(crc_part1, crc_part2, (off_t)len2);

    // step 2: Initialize and run deflate to produce compressed data
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
    int rc_def = deflate(&dstrm, 4);
    uLong compSize = dstrm.total_out;
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Initialize inflate, query header, and decompress the produced data
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, sizeof(gz_header));
    int rc_get_header = inflateGetHeader(&istrm, &head);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compSize;
    uLong decBound = sourceLen + 64;
    Bytef *decBuf = (Bytef *)malloc((size_t)decBound);
    memset(decBuf, 0, (size_t)decBound);
    istrm.next_out = decBuf;
    istrm.avail_out = (uInt)decBound;
    int rc_inflate = inflate(&istrm, 4);
    uLong inflated = istrm.total_out;
    uLong adler_dec = adler32(1UL, decBuf, (uInt)inflated);
    int rc_inf_end = inflateEnd(&istrm);

    // step 4: Write decompressed output to a gzFile, query direct mode, and cleanup
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, decBuf, (unsigned int)inflated);
    int is_direct = gzdirect(gf);
    int rc_gzclose = gzclose(gf);
    free(compBuf);
    free(decBuf);
    (void)adler_src;
    (void)crc_combined;
    (void)rc_def_init;
    (void)rc_def;
    (void)rc_def_end;
    (void)rc_inf_init;
    (void)rc_get_header;
    (void)rc_inflate;
    (void)adler_dec;
    (void)rc_inf_end;
    (void)rc_gzwrite;
    (void)is_direct;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}