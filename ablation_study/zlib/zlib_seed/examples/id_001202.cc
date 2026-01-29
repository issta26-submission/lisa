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
//<ID> 1202
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib API sequence test payload for gzdopen/gzeof/deflateCopy/adler32_z/inflateInit_";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    z_stream def_src;
    z_stream def_copy;
    z_stream istrm;
    memset(&def_src, 0, (size_t)sizeof(z_stream));
    memset(&def_copy, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    unsigned char *inbuf = new unsigned char[256];
    unsigned char *outbuf = new unsigned char[256];
    memset(inbuf, 0, (size_t)256);
    memset(outbuf, 0, (size_t)256);
    def_src.next_in = inbuf;
    def_src.avail_in = 0;
    def_src.next_out = outbuf;
    def_src.avail_out = (uInt)256;

    // step 2: Setup
    int rc_def_init = deflateInit_(&def_src, 6, version, (int)sizeof(z_stream));
    int rc_def_copy = deflateCopy(&def_copy, &def_src);
    istrm.next_in = inbuf;
    istrm.avail_in = 0;
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));

    // step 3: Core operations
    uLong adler = adler32_z(0UL, (const Bytef *)payload, (z_size_t)payload_len);
    gzFile gzf = gzdopen(1, "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)payload, (unsigned int)(payload_len ? payload_len : 0));
    int rc_gzeof = gzeof(gzf);

    // step 4: Validate and cleanup
    int rc_gzclose = gzclose(gzf);
    int rc_def_end_src = deflateEnd(&def_src);
    int rc_def_end_copy = deflateEnd(&def_copy);
    int rc_inf_end = inflateEnd(&istrm);
    delete [] inbuf;
    delete [] outbuf;
    (void)version;
    (void)rc_def_init;
    (void)rc_def_copy;
    (void)rc_inf_init;
    (void)adler;
    (void)rc_gzwrite;
    (void)rc_gzeof;
    (void)rc_gzclose;
    (void)rc_def_end_src;
    (void)rc_def_end_copy;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}