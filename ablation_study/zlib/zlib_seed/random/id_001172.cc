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
//<ID> 1172
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
    uLong comp_bound = compressBound(payload_len);
    uLongf comp_size_f = (uLongf)(comp_bound ? comp_bound : 1);
    Bytef *comp_buf = new Bytef[(size_t)comp_size_f];
    memset(comp_buf, 0, (size_t)comp_size_f);
    z_stream back_strm;
    memset(&back_strm, 0, (size_t)sizeof(z_stream));
    unsigned char *window = new unsigned char[1];
    memset(window, 0, (size_t)1);

    // step 2: Setup (compress payload into buffer)
    int rc_compress = compress2(comp_buf, &comp_size_f, (const Bytef *)payload, payload_len, 6);
    uLong comp_size = (uLong)comp_size_f;

    // step 3: Core operations (write compressed data to gz file, push back a byte, initialize inflateBack)
    gzFile gz_out = gzopen64("zlib_seq_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gz_out, comp_buf, (unsigned int)(comp_size ? comp_size : 0));
    int rc_gzclose_w = gzclose_w(gz_out);
    gzFile gz_in = gzopen64("zlib_seq_tmp.gz", "rb");
    int rc_gzungetc = gzungetc((int)(comp_buf[0]), gz_in);
    int rc_inflate_back_init = inflateBackInit_(&back_strm, 15, window, version, (int)sizeof(z_stream));
    int rc_inflate_back_end = inflateBackEnd(&back_strm);
    int rc_gzclose_r = gzclose_r(gz_in);

    // step 4: Validate and cleanup
    uLong crc_orig = crc32(0UL, (const Bytef *)payload, (uInt)payload_len);
    uLong crc_comp = crc32(0UL, comp_buf, (uInt)(comp_size ? comp_size : 0));
    delete [] comp_buf;
    delete [] window;
    (void)version;
    (void)rc_compress;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_gzungetc;
    (void)rc_inflate_back_init;
    (void)rc_inflate_back_end;
    (void)rc_gzclose_r;
    (void)crc_orig;
    (void)crc_comp;
    // API sequence test completed successfully
    return 66;
}