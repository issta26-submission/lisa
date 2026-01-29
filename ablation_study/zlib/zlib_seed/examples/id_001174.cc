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
//<ID> 1174
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib API sequence payload demonstrating gzopen64, gzwrite, gzungetc and inflateBackInit_";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    gzFile gzf_w = (gzFile)0;
    gzFile gzf_r = (gzFile)0;
    z_stream back_strm;
    memset(&back_strm, 0, (size_t)sizeof(z_stream));
    unsigned char *window = new unsigned char[(1 << 15)];
    memset(window, 0, (size_t)(1 << 15));

    // step 2: Setup (open for write and write payload)
    gzf_w = gzopen64("zlib_seq_test.gz", "wb");
    int rc_gzwrite = gzwrite(gzf_w, (voidpc)payload, (unsigned int)(payload_len ? payload_len : 0));
    int rc_gzclose_w = gzclose_w(gzf_w);

    // step 3: Core operations (open for read, push back one byte, read it, initialize inflateBack)
    gzf_r = gzopen64("zlib_seq_test.gz", "rb");
    int rc_gzungetc = gzungetc((int)payload[0], gzf_r);
    int rc_gzgetc = gzgetc(gzf_r);
    int rc_gzclose_r = gzclose_r(gzf_r);
    int rc_inflate_back_init = inflateBackInit_(&back_strm, 15, window, version, (int)sizeof(z_stream));
    int rc_inflate_back_end = inflateBackEnd(&back_strm);

    // step 4: Validate and cleanup
    delete [] window;
    (void)payload_len;
    (void)version;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_gzungetc;
    (void)rc_gzgetc;
    (void)rc_gzclose_r;
    (void)rc_inflate_back_init;
    (void)rc_inflate_back_end;
    // API sequence test completed successfully
    return 66;
}