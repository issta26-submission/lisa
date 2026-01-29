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
//<ID> 1173
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib API sequence payload for gz and inflateBackInit_ usage";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    gzFile gzf_out = (gzFile)0;
    gzFile gzf_in = (gzFile)0;
    z_stream back_strm;
    memset(&back_strm, 0, (size_t)sizeof(z_stream));
    unsigned char *window = new unsigned char[(1 << 15)];
    Bytef *read_buf = new Bytef[(size_t)(payload_len ? payload_len : 1)];
    memset(read_buf, 0, (size_t)(payload_len ? payload_len : 1));

    // step 2: Write payload to a gz file using gzopen64 and gzwrite
    gzf_out = gzopen64("zlib_seq_test.gz", "wb");
    int rc_gzwrite = gzwrite(gzf_out, (voidpc)payload, (unsigned int)(payload_len ? payload_len : 0));
    int rc_gzclose_w = gzclose(gzf_out);

    // step 3: Read back, use gzungetc, and initialize inflateBack for a realistic sequence
    gzf_in = gzopen64("zlib_seq_test.gz", "rb");
    int rc_ungetc = gzungetc((int)payload[0], gzf_in);
    int rc_gzread = gzread(gzf_in, (voidp)read_buf, (unsigned int)(payload_len ? payload_len : 0));
    int rc_inflateBackInit = inflateBackInit_(&back_strm, 15, window, version, (int)sizeof(z_stream));
    int rc_inflateBackEnd = inflateBackEnd(&back_strm);

    // step 4: Validate and cleanup
    uLong crc_orig = crc32(0UL, (const Bytef *)payload, (uInt)payload_len);
    uLong crc_read = crc32(0UL, read_buf, (uInt)(payload_len ? payload_len : 0));
    int rc_gzclose_r = gzclose(gzf_in);
    delete [] window;
    delete [] read_buf;
    (void)version;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_ungetc;
    (void)rc_gzread;
    (void)rc_inflateBackInit;
    (void)rc_inflateBackEnd;
    (void)crc_orig;
    (void)crc_read;
    // API sequence test completed successfully
    return 66;
}