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
//<ID> 602
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare data and deflate stream structure
    const char src[] = "zlib API sequence test payload for gzprintf and deflateInit2_";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();

    // step 2: Initialize deflate with explicit parameters (level, method, windowBits, memLevel, strategy)
    int rc_def_init = deflateInit2_(&dstrm, 6, 8 /* Z_DEFLATED */, 15, 8, 0 /* Z_DEFAULT_STRATEGY */, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;

    // step 3: Open a gzFile, write the payload via gzprintf (using the source as the format string),
    // and query the current offset with gzoffset64 to transfer a runtime value into the z_stream
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzprintf = gzprintf(gf, src);
    off64_t off = gzoffset64(gf);
    dstrm.total_out = (uLong)off;

    // step 4: Cleanup deflate stream and gzFile, record return codes to avoid unused warnings
    int rc_def_end = deflateEnd(&dstrm);
    int rc_gzclose = gzclose(gf);
    (void)rc_def_init;
    (void)rc_gzprintf;
    (void)off;
    (void)rc_def_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}