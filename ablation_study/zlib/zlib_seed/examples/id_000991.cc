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
//<ID> 991
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initial values (Initialize)
    const char payload[] = "zlib API sequence payload exercising gzprintf, gzwrite, inflatePrime and get_crc_table";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));

    // step 2: Stream initialization and priming (Configure)
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    int rc_inflate_prime = inflatePrime(&istrm, 3, 5);

    // step 3: Write compressed-friendly output using gzFile APIs (Operate)
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzprintf = gzprintf(gf, "%s", payload);
    int rc_gzwrite = gzwrite(gf, (voidpc)src_buf, (unsigned int)payload_len);
    int rc_gzclose = gzclose(gf);

    // step 4: Validate checksums and cleanup (Validate -> Cleanup)
    const z_crc_t *crc_table = get_crc_table();
    uLong seed = (uLong)crc_table[0];
    uLong crc_val = crc32(seed, src_buf, (uInt)payload_len);
    int rc_inflate_end = inflateEnd(&istrm);

    (void)rc_inflate_init;
    (void)rc_inflate_prime;
    (void)rc_gzprintf;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)crc_val;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}