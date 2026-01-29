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
//<ID> 996
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char payload[] = "zlib API sequence payload exercising gzprintf, gzwrite, inflatePrime and get_crc_table";
    unsigned int payload_len = (unsigned int)(sizeof(payload) - 1);
    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&strm, zlibVersion(), (int)sizeof(z_stream));
    const z_crc_t *crc_tbl = get_crc_table();
    uLong first_crc_entry = (uLong)crc_tbl[0];

    // step 2: Configure
    int rc_inflate_prime = inflatePrime(&strm, 1, 1);
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");

    // step 3: Operate
    int rc_gzprintf = gzprintf(gf, "%s", payload);
    int rc_gzwrite = gzwrite(gf, (voidpc)payload, payload_len);

    // step 4: Validate and Cleanup
    int rc_gzclose = gzclose(gf);
    int rc_inflate_end = inflateEnd(&strm);

    (void)rc_inflate_init;
    (void)first_crc_entry;
    (void)rc_inflate_prime;
    (void)rc_gzprintf;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}