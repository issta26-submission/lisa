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
//<ID> 1145
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib API sequence payload for compressBound/adler32/gz usage";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    uLong bound = compressBound(payload_len);
    Bytef *buf = new Bytef[(bound ? bound : 1)];
    memset(buf, 0, (size_t)(bound ? bound : 1));
    memcpy(buf, payload, (size_t)payload_len);
    gzFile gzf = gzopen("zlib_api_sequence_test.gz", "wb");
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));

    // step 2: Setup / Configure
    uLong computed_bound = bound;
    (void)computed_bound;

    // step 3: Core operations (compute adler and prepare inflate cleanup)
    uLong adler = adler32_z(0UL, (const Bytef *)buf, (z_size_t)payload_len);
    (void)adler;

    // step 4: Validate and cleanup
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_gzclose_w = gzclose_w(gzf);
    delete [] buf;
    (void)rc_inflate_end;
    (void)rc_gzclose_w;
    // API sequence test completed successfully
    return 66;
}