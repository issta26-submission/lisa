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
//<ID> 1147
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload for compress -> write gz -> inflate lifecycle";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));

    // step 2: Setup (compute bounds, allocate buffer, compress)
    uLong bound = compressBound(payload_len);
    Bytef *comp_buf = new Bytef[(size_t)(bound ? bound : 1)];
    uLongf comp_len = (uLongf)(bound ? bound : 1);
    int rc_compress = compress(comp_buf, &comp_len, (const Bytef *)payload, payload_len);

    // step 3: Core operations (checksum compressed data, write to gz, initialize and end inflate)
    uLong checksum = adler32_z(0UL, comp_buf, (z_size_t)comp_len);
    gzFile gzf = gzopen("zlib_api_sequence_test.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, comp_buf, (unsigned int)(comp_len ? comp_len : 0));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_gzclose_w = gzclose_w(gzf);

    // step 4: Cleanup and validation
    delete [] comp_buf;
    (void)version;
    (void)rc_compress;
    (void)checksum;
    (void)rc_gzwrite;
    (void)rc_inflate_init;
    (void)rc_inflate_end;
    (void)rc_gzclose_w;
    // API sequence test completed successfully
    return 66;
}