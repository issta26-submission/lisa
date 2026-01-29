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
//<ID> 978
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initial payload
    const char payload[] = "zlib sample payload for gzprintf and adler32_z demo";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef * src_buf = (const Bytef *)payload;
    Bytef * out_buf = new Bytef[(payload_len ? payload_len : 1)];

    // step 2: Initialize inflate stream (Initialize -> Configure)
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = (Bytef *)src_buf;
    istrm.avail_in = (uInt)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);

    // step 3: Operate — compute checksum, write via gzprintf, query offset
    uLong checksum = adler32_z(0UL, src_buf, (z_size_t)payload_len);
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "w+b");
    int rc_gzprintf = gzprintf(gf, "%s", payload);
    off64_t offset = gzoffset64(gf);

    // step 4: Cleanup and validate (Cleanup)
    int rc_gzclose = gzclose(gf);
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] out_buf;

    (void)rc_inflate_init;
    (void)checksum;
    (void)rc_gzprintf;
    (void)offset;
    (void)rc_gzclose;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}