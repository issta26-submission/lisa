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
//<ID> 858
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initial CRC computation + write to gz file
    const char src[] = "zlib api sequence payload: compute crc, gzprintf, compress, inflateInit_, crc32_combine";
    uLong srcLen = (uLong)(sizeof(src) - 1);
    const Bytef *source = (const Bytef *)src;
    uLong crc_initial = crc32_z(0L, source, (z_size_t)srcLen);
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gz_printf = gzprintf(gf, "%s", source);
    gzclearerr(gf);

    // step 2: Compress the source into a buffer and compute CRC of compressed data
    uLong bound = compressBound(srcLen);
    uLongf compLen = (uLongf)bound;
    Bytef *compBuf = new Bytef[(size_t)(bound ? bound : 1)];
    int rc_comp = compress2(compBuf, &compLen, source, srcLen, 6);
    uLong crc_comp = crc32_z(0L, compBuf, (z_size_t)compLen);
    uLong crc_combined = crc32_combine(crc_initial, crc_comp, (off_t)compLen);

    // step 3: Initialize inflate stream with compressed data, then validate and end
    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(z_stream));
    strm.next_in = compBuf;
    strm.avail_in = (uInt)compLen;
    int rc_inflate_init = inflateInit_(&strm, zlibVersion(), (int)sizeof(z_stream));
    int rc_inflate_end = inflateEnd(&strm);

    // step 4: Cleanup resources and finalize
    int rc_close = gzclose(gf);
    delete [] compBuf;
    (void)crc_initial;
    (void)rc_gz_printf;
    (void)rc_comp;
    (void)crc_comp;
    (void)crc_combined;
    (void)rc_inflate_init;
    (void)rc_inflate_end;
    (void)rc_close;
    // API sequence test completed successfully
    return 66;
}