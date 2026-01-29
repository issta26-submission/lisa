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
//<ID> 1177
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib API sequence payload for compress/gz and inflateBackInit_ usage";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(payload_len);
    uLongf comp_len = (uLongf)(bound ? bound : 1);
    Bytef *comp_buf = new Bytef[(size_t)comp_len];
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    unsigned char *window = new unsigned char[(1 << 15)];

    // step 2: Setup (compress payload and initialize inflateBack)
    int rc_compress = compress2(comp_buf, &comp_len, (const Bytef *)payload, payload_len, 6);
    int rc_inflate_back_init = inflateBackInit_(&istrm, 15, window, version, (int)sizeof(z_stream));

    // step 3: Core operations (write compressed data to gz file, then reopen and ungetc)
    gzFile gzf_write = gzopen64("zlib_seq_test.gz", "wb");
    int rc_gzwrite = gzwrite(gzf_write, comp_buf, (unsigned int)(comp_len ? comp_len : 0));
    int rc_gzclose_w = gzclose(gzf_write);
    gzFile gzf_read = gzopen64("zlib_seq_test.gz", "rb");
    int rc_gzungetc = gzungetc((int)'X', gzf_read);
    int rc_gzclose_r = gzclose(gzf_read);

    // step 4: Cleanup and validate-like handling
    int rc_inflate_back_end = inflateBackEnd(&istrm);
    delete [] comp_buf;
    delete [] window;
    (void)payload_len;
    (void)version;
    (void)bound;
    (void)rc_compress;
    (void)rc_inflate_back_init;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_gzungetc;
    (void)rc_gzclose_r;
    (void)rc_inflate_back_end;
    // API sequence test completed successfully
    return 66;
}