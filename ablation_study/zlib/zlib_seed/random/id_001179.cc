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
//<ID> 1179
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib API sequence payload for gzopen64/gzwrite/gzungetc and inflateBackInit_ usage";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    unsigned char *window = new unsigned char[(1 << 15)];
    memset(window, 0, (size_t)(1 << 15));

    // step 2: Setup (initialize inflate-back stream)
    int rc_inflateback_init = inflateBackInit_(&istrm, 15, window, version, (int)sizeof(z_stream));

    // step 3: Core operations (open gz file, write payload, push back one byte)
    gzFile gzf = gzopen64("zlib_seq_out.gz", "wb+");
    int rc_gzwrite = gzwrite(gzf, (voidpc)payload, (unsigned int)(payload_len ? payload_len : 0));
    int rc_gzungetc = gzungetc((int)payload[0], gzf);

    // step 4: Validate and cleanup
    int rc_gzclose_w = gzclose_w(gzf);
    int rc_inflateback_end = inflateBackEnd(&istrm);
    int rc_inflate_end = inflateEnd(&istrm);
    uLong crc_orig = crc32(0UL, (const Bytef *)payload, (uInt)(payload_len ? payload_len : 0));
    delete [] window;
    (void)version;
    (void)rc_inflateback_init;
    (void)rc_gzwrite;
    (void)rc_gzungetc;
    (void)rc_gzclose_w;
    (void)rc_inflateback_end;
    (void)rc_inflate_end;
    (void)crc_orig;
    // API sequence test completed successfully
    return 66;
}