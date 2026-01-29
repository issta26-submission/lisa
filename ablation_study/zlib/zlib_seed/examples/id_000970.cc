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
//<ID> 970
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and Initialize
    const char payload[] = "zlib API sequence payload exercising inflateInit_, adler32_z, gzprintf and gzoffset64";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef * src_buf = (const Bytef *)payload;
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure and Operate - checksum and write via gzFile
    uLong checksum1 = adler32_z(0UL, src_buf, (z_size_t)payload_len);
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "w+b");
    int rc_gzprintf = gzprintf(gf, payload);
    off64_t offset64 = gzoffset64(gf);
    int rc_gzclose = gzclose(gf);

    // step 3: Validate - chain checksum and capture values
    uLong checksum2 = adler32_z(checksum1, src_buf, (z_size_t)payload_len);
    uLong combined_preview = checksum1 ^ checksum2 ^ (uLong)offset64;

    // step 4: Cleanup
    int rc_inflate_end = inflateEnd(&istrm);

    (void)rc_inflate_init;
    (void)checksum1;
    (void)rc_gzprintf;
    (void)offset64;
    (void)rc_gzclose;
    (void)checksum2;
    (void)combined_preview;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}