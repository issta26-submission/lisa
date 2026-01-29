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
//<ID> 1171
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
    uLong bound = compressBound(payload_len);
    uLongf comp_len_f = (uLongf)(bound ? bound : 1);
    Bytef *comp_buf = new Bytef[(size_t)(bound ? bound : 1)];
    memset(comp_buf, 0, (size_t)(bound ? bound : 1));
    int rc_compress = compress2(comp_buf, &comp_len_f, (const Bytef *)payload, payload_len, 6);
    uLong comp_size = (uLong)comp_len_f;

    // step 2: Setup (write compressed data to a gz file using gzopen64 and gzwrite)
    gzFile gz_w = gzopen64("zlib_seq_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gz_w, comp_buf, (unsigned int)(comp_size ? comp_size : 0));
    int rc_gzclose_w = gzclose_w(gz_w);

    // step 3: Operate (open for read, read, push back a byte with gzungetc and read again)
    gzFile gz_r = gzopen64("zlib_seq_tmp.gz", "rb");
    uInt read_buf_size = (uInt)(comp_size ? comp_size : 1);
    Bytef *read_buf = new Bytef[(size_t)read_buf_size];
    memset(read_buf, 0, (size_t)read_buf_size);
    int rc_gzread1 = gzread(gz_r, read_buf, (unsigned int)read_buf_size);
    int push_char = (int)read_buf[(read_buf_size ? read_buf_size : 1) - 1];
    int rc_gzungetc = gzungetc(push_char, gz_r);
    Bytef one_char_buf[1];
    memset(one_char_buf, 0, 1);
    int rc_gzread2 = gzread(gz_r, one_char_buf, 1);
    int rc_gzclose_r = gzclose_r(gz_r);

    // step 4: Configure/Validate/Cleanup (initialize inflate back, teardown, free resources)
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    unsigned char *window = new unsigned char[32768];
    memset(window, 0, 32768);
    int rc_inflate_back_init = inflateBackInit_(&istrm, 15, window, version, (int)sizeof(z_stream));
    int rc_inflate_back_end = inflateBackEnd(&istrm);
    delete [] comp_buf;
    delete [] read_buf;
    delete [] window;
    (void)version;
    (void)rc_compress;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_gzread1;
    (void)rc_gzungetc;
    (void)rc_gzread2;
    (void)rc_gzclose_r;
    (void)rc_inflate_back_init;
    (void)rc_inflate_back_end;
    // API sequence test completed successfully
    return 66;
}