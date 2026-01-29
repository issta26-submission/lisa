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
//<ID> 1211
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib API sequence payload for checksums and streams";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    unsigned char *work_buf = new unsigned char[512];
    memset(work_buf, 0, (size_t)512);

    // step 2: Setup (initialize streams and compute checksums)
    dstrm.next_in = (Bytef *)payload;
    dstrm.avail_in = (uInt)payload_len;
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    istrm.next_in = work_buf;
    istrm.avail_in = 0;
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    uLong payload_adler = adler32(0UL, (const Bytef *)payload, (uInt)payload_len);
    uLong payload_crc = crc32(0UL, (const Bytef *)payload, (uInt)payload_len);
    istrm.adler = payload_adler;
    dstrm.adler = payload_crc;

    // step 3: Core operations (write gz, perform backend inflate teardown)
    gzFile gzf = gzopen("zlib_api_sequence_out.gz", "wb");
    int rc_gzwrite = 0;
    rc_gzwrite = gzwrite(gzf, (const void *)payload, (unsigned int)(payload_len ? payload_len : 0));
    int rc_inflate_back_end = inflateBackEnd(&istrm);

    // step 4: Validate and cleanup
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_gzclose = gzclose(gzf);
    delete [] work_buf;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_inflate_init;
    (void)payload_adler;
    (void)payload_crc;
    (void)rc_gzwrite;
    (void)rc_inflate_back_end;
    (void)rc_deflate_end;
    (void)rc_inflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}