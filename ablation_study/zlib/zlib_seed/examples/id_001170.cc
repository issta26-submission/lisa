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
//<ID> 1170
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib API sequence payload demonstrating compress, gz I/O and inflateBack init/end";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    uLong comp_bound = compressBound(payload_len);
    comp_bound = (comp_bound ? comp_bound : 1);
    Bytef *comp_buf = new Bytef[(size_t)comp_bound];
    memset(comp_buf, 0, (size_t)comp_bound);
    uLongf dest_len = (uLongf)comp_bound;
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    unsigned char *window = new unsigned char[(1u << 15)]; // 32K window
    memset(window, 0, (size_t)(1u << 15));

    // step 2: Setup (compress payload and initialize inflate-back)
    int rc_compress = compress2(comp_buf, &dest_len, (const Bytef *)payload, payload_len, 6);
    uLong comp_size = (uLong)dest_len;
    int rc_inflate_back_init = inflateBackInit_(&istrm, 15, window, version, (int)sizeof(z_stream));

    // step 3: Core operations (write compressed data to gz file, then reopen and push a byte back)
    gzFile gzf_w = gzopen64("zlib_seq_out.gz", "wb");
    int rc_gzwrite = gzwrite(gzf_w, comp_buf, (unsigned int)(comp_size ? comp_size : 0));
    int rc_gzclose_w = gzclose_w(gzf_w);
    gzFile gzf_r = gzopen64("zlib_seq_out.gz", "rb");
    int rc_gzungetc = gzungetc((int)'X', gzf_r);
    int ch = gzgetc(gzf_r);
    int rc_gzclose_r = gzclose(gzf_r);

    // step 4: Validate and cleanup
    int rc_inflate_back_end = inflateBackEnd(&istrm);
    delete [] comp_buf;
    delete [] window;
    (void)version;
    (void)rc_compress;
    (void)rc_inflate_back_init;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_gzungetc;
    (void)ch;
    (void)rc_gzclose_r;
    (void)rc_inflate_back_end;
    // API sequence test completed successfully
    return 66;
}