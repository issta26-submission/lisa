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
//<ID> 1146
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "zlib API sequence payload demonstrating gzopen/gzclose_w/inflateEnd/compressBound/adler32_z";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    memcpy(comp_buf, payload, (size_t)payload_len);

    // step 2: Configure (open gz for writing and write payload)
    gzFile gzf = gzopen("zlib_api_sequence_test.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, comp_buf, (unsigned int)payload_len);

    // step 3: Operate and validate (initialize inflate stream, compute adler32_z checksum)
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)payload_len;
    istrm.next_out = comp_buf;
    istrm.avail_out = (uInt)(bound ? bound : 1);
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    uLong checksum = adler32_z(0UL, comp_buf, (z_size_t)payload_len);

    // step 4: Cleanup
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_gzclose_w = gzclose_w(gzf);
    delete [] comp_buf;
    (void)rc_gzwrite;
    (void)rc_inflate_init;
    (void)checksum;
    (void)rc_inflate_end;
    (void)rc_gzclose_w;
    // API sequence test completed successfully
    return 66;
}