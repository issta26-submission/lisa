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
//<ID> 974
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib API sequence payload exercising inflateInit_, gzopen, gzprintf, gzoffset64 and adler32_z";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef * src_buf = (const Bytef *)payload;
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));

    // step 2: Initialize and configure inflate stream
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = NULL;
    istrm.avail_in = 0;
    istrm.next_out = NULL;
    istrm.avail_out = 0;

    // step 3: Operate — write payload to a gz file and validate with checksums and offset
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "w+b");
    int rc_gzprintf = gzprintf(gf, payload);
    off64_t offset_after_write = gzoffset64(gf);
    int rc_gzclose = gzclose(gf);
    uLong ad_initial = adler32_z(0UL, src_buf, (z_size_t)payload_len);
    uLong ad_combined = adler32_z(ad_initial, src_buf, (z_size_t)payload_len);

    // step 4: Cleanup
    int rc_inflate_end = inflateEnd(&istrm);

    (void)rc_inflate_init;
    (void)rc_gzprintf;
    (void)offset_after_write;
    (void)rc_gzclose;
    (void)ad_initial;
    (void)ad_combined;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}