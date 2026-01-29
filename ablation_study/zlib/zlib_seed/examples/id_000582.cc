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
//<ID> 582
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compute checksums
    const char src[] = "zlib fuzz payload: initialize, checksum, compress, write, cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong adler = adler32(0UL, source, (uInt)sourceLen);
    uLong crc1 = crc32(0UL, source, (uInt)(sourceLen / 2));
    uLong crc2 = crc32(0UL, source + (sourceLen / 2), (uInt)(sourceLen - (sourceLen / 2)));
    uLong crc_combined = crc32_combine(crc1, crc2, (off_t)sourceLen);

    // step 2: Compress the source into a buffer
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 3: Initialize and configure zlib streams; probe inflate header
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_set_dict = deflateSetDictionary(&dstrm, source, (uInt)4);

    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    gz_header header;
    memset(&header, 0, sizeof(gz_header));
    int rc_get_header = inflateGetHeader(&istrm, &header);

    // step 4: Persist compressed data via gzFile, query direct mode, and cleanup
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, compBuf, (unsigned int)compLen);
    int direct = gzdirect(gf);
    int rc_gzclose = gzclose(gf);
    int rc_def_end = deflateEnd(&dstrm);
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    (void)adler;
    (void)crc_combined;
    (void)rc_comp;
    (void)rc_def_init;
    (void)rc_set_dict;
    (void)rc_inf_init;
    (void)rc_get_header;
    (void)rc_gzwrite;
    (void)direct;
    (void)rc_gzclose;
    (void)rc_def_end;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}