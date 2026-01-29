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
//<ID> 1142
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "zlib API sequence payload for compress/gz/inflate usage";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    uLong adler_orig = adler32_z(0UL, (const Bytef *)payload, (z_size_t)payload_len);
    uLong bound = compressBound(payload_len);
    uLongf comp_len = (uLongf)bound;
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));

    // step 2: Setup (compress the payload and open gz for writing)
    int rc_compress = compress(comp_buf, &comp_len, (const Bytef *)payload, payload_len);
    gzFile gzf = gzopen("zlib_api_sequence_test.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, comp_buf, (unsigned int)(comp_len ? comp_len : 0));

    // step 3: Core operations (initialize inflate and then end it)
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    int rc_inflate_end = inflateEnd(&istrm);

    // step 4: Validate and cleanup
    uLong adler_comp = adler32_z(0UL, (const Bytef *)comp_buf, (z_size_t)(comp_len ? comp_len : 0));
    int rc_gzclose_w = gzclose_w(gzf);
    delete [] comp_buf;
    (void)adler_orig;
    (void)adler_comp;
    (void)rc_compress;
    (void)rc_gzwrite;
    (void)rc_inflate_init;
    (void)rc_inflate_end;
    (void)rc_gzclose_w;
    // API sequence test completed successfully
    return 66;
}