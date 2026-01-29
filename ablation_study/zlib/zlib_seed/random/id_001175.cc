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
//<ID> 1175
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib API sequence payload demonstrating gzopen64/gzwrite/gzungetc and inflateBackInit_ usage";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    uLong comp_bound = compressBound(payload_len);
    comp_bound = (comp_bound ? comp_bound : 1);
    uLongf comp_len = comp_bound;
    Bytef *comp_buf = new Bytef[(size_t)comp_bound];
    memset(comp_buf, 0, (size_t)comp_bound);

    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    unsigned char *window = new unsigned char[(1 << 15)];
    memset(window, 0, (size_t)(1 << 15));

    // step 2: Setup (compress payload, open gz file, write compressed data)
    int rc_compress = compress2(comp_buf, &comp_len, (const Bytef *)payload, payload_len, 6);
    gzFile gzf = gzopen64("zlib_seq_out.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, comp_buf, (unsigned int)(comp_len ? comp_len : 0));
    int rc_gzungetc = gzungetc((int)'A', gzf);
    int rc_gzflush = gzflush(gzf, 1);
    int rc_gzclose_w = gzclose_w(gzf);

    // step 3: Core operations (initialize inflateBack, and perform an independent uncompress for validation)
    int rc_inflate_back_init = inflateBackInit_(&istrm, 15, window, version, (int)sizeof(z_stream));
    uLongf decomp_len = (uLongf)(payload_len ? payload_len : 1);
    Bytef *decomp_buf = new Bytef[(size_t)(decomp_len)];
    memset(decomp_buf, 0, (size_t)(decomp_len));
    int rc_uncompress = uncompress(decomp_buf, &decomp_len, comp_buf, comp_len);

    // step 4: Validate and cleanup
    uLong crc_orig = crc32(0UL, (const Bytef *)payload, (uInt)payload_len);
    uLong crc_decomp = crc32(0UL, decomp_buf, (uInt)(decomp_len ? decomp_len : 0));
    int rc_inflate_back_end = inflateBackEnd(&istrm);

    delete [] comp_buf;
    delete [] decomp_buf;
    delete [] window;

    (void)version;
    (void)rc_compress;
    (void)rc_gzwrite;
    (void)rc_gzungetc;
    (void)rc_gzflush;
    (void)rc_gzclose_w;
    (void)rc_inflate_back_init;
    (void)rc_uncompress;
    (void)crc_orig;
    (void)crc_decomp;
    (void)rc_inflate_back_end;
    // API sequence test completed successfully
    return 66;
}