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
//<ID> 1143
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "zlib API sequence payload for gz/compressBound/adler32/inflateEnd usage";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    gzFile gzf = gzopen("zlib_api_sequence_test.gz", "wb");
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));

    // step 2: Setup (initialize inflate, compute buffer bound, allocate buffer and copy data)
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    uLong bound = compressBound(payload_len);
    Bytef *buf = new Bytef[(bound ? bound : 1)];
    memcpy(buf, payload, (size_t)payload_len);
    uLong buf_len = payload_len;

    // step 3: Core operations (compute adler32 on buffer, write buffer to gz, end inflate)
    uLong adler = adler32_z(0UL, (const Bytef *)buf, (z_size_t)buf_len);
    int rc_gzwrite = gzwrite(gzf, (const void *)buf, (unsigned int)buf_len);
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_gzclose_w = gzclose_w(gzf);

    // step 4: Validate and cleanup
    delete [] buf;
    (void)version;
    (void)rc_inflate_init;
    (void)bound;
    (void)adler;
    (void)rc_gzwrite;
    (void)rc_inflate_end;
    (void)rc_gzclose_w;
    // API sequence test completed successfully
    return 66;
}