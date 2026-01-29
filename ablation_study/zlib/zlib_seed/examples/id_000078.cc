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
//<ID> 78
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream inf_strm;
    z_stream def_strm;
    memset(&inf_strm, 0, sizeof(z_stream));
    memset(&def_strm, 0, sizeof(z_stream));
    const char src_c[] = "Example payload for zlib API sequence: compress -> write gz -> inflate -> crc";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    uLongf compLen = (uLongf)bound;
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    Bytef *decompBuf = (Bytef *)malloc((size_t)sourceLen);
    const char *version = zlibVersion();

    // step 2: Configure (deflate init & params, compress, write to gzFile and get offset)
    int rc_deflate_init = deflateInit_(&def_strm, -1, version, (int)sizeof(z_stream));
    int rc_deflate_params = deflateParams(&def_strm, 6, 0);
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gzwrite = 0;
    off64_t offset = (off64_t)(-1);
    rc_gzwrite = gzwrite(gz, compBuf, (unsigned int)compLen);
    offset = gzoffset64(gz);

    // step 3: Operate & Validate (initialize inflate, inflate compressed data, compute crc)
    int rc_inflate_init = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    inf_strm.next_in = compBuf;
    inf_strm.avail_in = (uInt)compLen;
    inf_strm.next_out = decompBuf;
    inf_strm.avail_out = (uInt)sourceLen;
    int rc_inflate = inflate(&inf_strm, 4); /* Z_FINISH == 4 */
    uLong crc = crc32_z(0UL, decompBuf, (z_size_t)inf_strm.total_out);

    // step 4: Cleanup
    (void)rc_deflate_init; (void)rc_deflate_params; (void)rc_comp; (void)rc_gzwrite;
    (void)offset; (void)rc_inflate_init; (void)rc_inflate; (void)crc;
    inflateEnd(&inf_strm);
    deflateEnd(&def_strm);
    gzclose(gz);
    free(compBuf);
    free(decompBuf);

    // API sequence test completed successfully
    return 66;
}