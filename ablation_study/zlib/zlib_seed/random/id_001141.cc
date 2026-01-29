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
//<ID> 1141
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "zlib API sequence payload for gz and inflateEnd usage";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    uLong checksum = adler32_z(0UL, (const Bytef *)payload, (z_size_t)payload_len);
    uLong bound = compressBound(payload_len);
    Bytef *buffer = new Bytef[(bound ? bound : 1)];
    memcpy(buffer, payload, (size_t)payload_len);

    // step 2: Setup (open gz file and write payload)
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, buffer, (unsigned int)payload_len);

    // step 3: Configure and operate (initialize inflate stream and end it)
    z_stream istrm2;
    memset(&istrm2, 0, (size_t)sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inflate_init = inflateInit_(&istrm2, version, (int)sizeof(z_stream));
    int rc_inflate_end = inflateEnd(&istrm2);

    // step 4: Validate and cleanup
    int rc_gzclose = gzclose_w(gzf);
    delete [] buffer;
    (void)checksum;
    (void)bound;
    (void)istrm;
    (void)rc_gzwrite;
    (void)rc_inflate_init;
    (void)rc_inflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}