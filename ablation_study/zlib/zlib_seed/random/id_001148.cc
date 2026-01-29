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
//<ID> 1148
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
    gzFile gzf = gzopen("zlib_api_sequence_test.gz", "wb");
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));

    // step 2: Setup (compute bounds and allocate buffers)
    uLong bound = compressBound(payload_len);
    uLongf comp_buf_len = (uLongf)bound;
    Bytef *comp_buf = new Bytef[(comp_buf_len ? comp_buf_len : 1)];
    uLongf actual_comp_len = comp_buf_len;
    int rc_compress = compress2(comp_buf, &actual_comp_len, (const Bytef *)payload, payload_len, 6);

    // step 3: Core operations (write compressed data to gz, initialize and finalize inflate)
    int rc_gzwrite = gzwrite(gzf, comp_buf, (unsigned int)(actual_comp_len ? actual_comp_len : 0));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    int rc_inflate_end = inflateEnd(&istrm);

    // step 4: Validate and cleanup
    uLong adler_orig = adler32_z(0UL, (const Bytef *)payload, (z_size_t)payload_len);
    uLong adler_comp = adler32_z(0UL, comp_buf, (z_size_t)actual_comp_len);
    int rc_gzclose = gzclose_w(gzf);
    delete [] comp_buf;
    (void)version;
    (void)bound;
    (void)rc_compress;
    (void)rc_gzwrite;
    (void)rc_inflate_init;
    (void)rc_inflate_end;
    (void)adler_orig;
    (void)adler_comp;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}