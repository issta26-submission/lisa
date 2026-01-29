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
//<ID> 1136
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "zlib API sequence payload for deflate/inflate/back usage";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    z_stream dstrm;
    z_stream istrm;
    z_stream bstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    memset(&bstrm, 0, (size_t)sizeof(z_stream));
    gzFile gzf = gzopen("zlib_api_sequence_test.gz", "wb");

    // step 2: Setup (initialize deflate and compute bounds)
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    dstrm.next_in = (Bytef *)payload;
    dstrm.avail_in = (uInt)payload_len;
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)(bound ? bound : 1);

    // step 3: Core operations (deflate, write gz, initialize inflate/back and inflate)
    int rc_deflate = deflate(&dstrm, 0);
    uInt comp_size = (uInt)dstrm.total_out;
    int rc_gzwrite = gzwrite(gzf, comp_buf, comp_size);
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = comp_size;
    Bytef *out_buf = new Bytef[(payload_len + 64) ? (payload_len + 64) : 1];
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)(payload_len + 64);
    int rc_inflate = inflate(&istrm, 0);
    unsigned char *window = new unsigned char[32768];
    int rc_inflate_back_init = inflateBackInit_(&bstrm, 15, window, version, (int)sizeof(z_stream));
    int rc_inflate_back_end = inflateBackEnd(&bstrm);

    // step 4: Validate and cleanup
    uLong crc_orig = crc32(0UL, (const Bytef *)payload, (uInt)payload_len);
    uLong crc_decomp = crc32(0UL, (const Bytef *)out_buf, (uInt)istrm.total_out);
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_gzclose_r = gzclose_r(gzf);
    delete [] comp_buf;
    delete [] out_buf;
    delete [] window;
    (void)rc_deflate_init;
    (void)bound;
    (void)rc_deflate;
    (void)comp_size;
    (void)rc_gzwrite;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_inflate_back_init;
    (void)rc_inflate_back_end;
    (void)crc_orig;
    (void)crc_decomp;
    (void)rc_inflate_end;
    (void)rc_deflate_end;
    (void)rc_gzclose_r;
    // API sequence test completed successfully
    return 66;
}