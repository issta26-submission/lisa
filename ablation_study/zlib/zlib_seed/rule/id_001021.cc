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
//<ID> 1021
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    const char *ver = zlibVersion();
    uLong flags = zlibCompileFlags();
    const uLong ORIG_LEN = 256UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'A', (size_t)ORIG_LEN);
    uLong crc_orig = crc32_z(0UL, orig, (z_size_t)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);

    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = Z_NULL;
    dstrm.zfree = Z_NULL;
    dstrm.opaque = Z_NULL;
    deflateInit_(&dstrm, Z_DEFAULT_COMPRESSION, ver, (int)sizeof(z_stream));
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)ORIG_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, Z_FINISH);
    uLong comp_size = dstrm.total_out;
    deflateEnd(&dstrm);

    const char *tmpname = "zlib_fuzz_temp.gz";
    gzFile gw = gzopen(tmpname, "wb");
    gzwrite(gw, comp, (unsigned int)comp_size);
    gzungetc('X', gw);
    gzclose(gw);

    uLong crc_comp = crc32_z(0UL, comp, (z_size_t)comp_size);
    gzFile gr = gzopen(tmpname, "rb");
    gzungetc('Y', gr);
    gzclose(gr);
    remove(tmpname);

    free(orig);
    free(comp);
    free(decomp);
    (void)ver;
    (void)flags;
    (void)crc_orig;
    (void)crc_comp;
    return 66;
}