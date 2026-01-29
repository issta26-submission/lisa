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
//<ID> 1144
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "zlib API sequence payload demonstrating compress -> gz write -> inflate -> checksum";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(payload_len);
    uLongf comp_len = (uLongf)(bound ? bound : 1);
    Bytef *comp_buf = new Bytef[(comp_len ? comp_len : 1)];
    Bytef *out_buf = new Bytef[(payload_len ? payload_len : 1)];
    gzFile gzf = gzopen("zlib_api_sequence_test.gz", "wb");
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));

    // step 2: Setup (compress the payload and write compressed data to gz)
    int rc_compress = compress(comp_buf, &comp_len, (const Bytef *)payload, payload_len);
    int rc_gzwrite = gzwrite(gzf, comp_buf, (unsigned int)(comp_len ? comp_len : 0));

    // step 3: Configure and operate (initialize inflate, set buffers, inflate)
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(comp_len ? comp_len : 0);
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)(payload_len ? payload_len : 1);
    int rc_inflate = inflate(&istrm, 0);

    // step 4: Validate and cleanup
    uLong checksum = adler32_z(0UL, (const Bytef *)out_buf, (z_size_t)payload_len);
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_gzclose_w = gzclose_w(gzf);
    delete [] comp_buf;
    delete [] out_buf;
    (void)rc_compress;
    (void)rc_gzwrite;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)checksum;
    (void)rc_inflate_end;
    (void)rc_gzclose_w;
    // API sequence test completed successfully
    return 66;
}