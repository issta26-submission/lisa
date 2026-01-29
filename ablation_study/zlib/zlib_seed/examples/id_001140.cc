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
//<ID> 1140
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
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    uLong checksum_payload = adler32_z(1UL, (const Bytef *)payload, (z_size_t)payload_len);

    // step 2: Setup (compute compress bound and allocate buffers)
    uLong bound = compressBound(payload_len);
    uLongf comp_buf_len = (uLongf)(bound ? bound : 1);
    Bytef *comp_buf = new Bytef[(comp_buf_len ? comp_buf_len : 1)];
    uLongf actual_comp_len = comp_buf_len;

    // step 3: Core operations (compress payload, write to gz, init+end inflate)
    int rc_compress = compress(comp_buf, &actual_comp_len, (const Bytef *)payload, payload_len);
    gzFile gzf = gzopen("zlib_api_sequence_test.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, comp_buf, (unsigned int)(actual_comp_len ? actual_comp_len : 0));
    uLong checksum_comp = adler32_z(0UL, comp_buf, (z_size_t)(actual_comp_len ? actual_comp_len : 0));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    int rc_inflate_end = inflateEnd(&istrm);

    // step 4: Validate and cleanup
    int rc_gzclose_w = gzclose_w(gzf);
    delete [] comp_buf;
    (void)checksum_payload;
    (void)checksum_comp;
    (void)rc_compress;
    (void)rc_gzwrite;
    (void)rc_inflate_init;
    (void)rc_inflate_end;
    (void)rc_gzclose_w;
    // API sequence test completed successfully
    return 66;
}