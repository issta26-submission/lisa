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
//<ID> 1212
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib API sequence payload for checksums, deflate, inflateBackEnd and gzclose";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    unsigned char *in_window = new unsigned char[512];
    memset(in_window, 0, (size_t)512);
    gzFile gzf = gzopen("zlib_api_sequence_output.gz", "wb");

    // step 2: Setup (initialize deflate and inflate streams, compute checksums)
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    uLong payload_adler = adler32(0UL, (const Bytef *)payload, (uInt)payload_len);
    uLong payload_crc = crc32(0UL, (const Bytef *)payload, (uInt)payload_len);
    istrm.next_in = in_window;
    istrm.avail_in = 0;
    istrm.adler = payload_adler;

    // step 3: Core operations (write payload to gz, simulate deflate usage, teardown inflate back-end)
    int rc_gzwrite = gzwrite(gzf, (voidpc)payload, (unsigned int)(payload_len ? payload_len : 0));
    dstrm.total_in = payload_len;
    dstrm.adler = payload_adler;
    dstrm.next_in = (Bytef *)payload;
    dstrm.avail_in = (uInt)payload_len;
    int rc_inflate_backend = inflateBackEnd(&istrm);

    // step 4: Validate and cleanup
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_gzclose = gzclose(gzf);
    delete [] in_window;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_inflate_init;
    (void)payload_adler;
    (void)payload_crc;
    (void)rc_gzwrite;
    (void)rc_inflate_backend;
    (void)rc_deflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}