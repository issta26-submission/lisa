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
//<ID> 1159
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib API sequence payload for combined deflate/inflate and gz usage";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    z_stream dstrm;
    z_stream istrm;
    z_stream istrm_copy;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm_copy, 0, (size_t)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, (size_t)sizeof(gz_header));
    head.text = 1;
    head.os = 255;

    // step 2: Setup (initialize deflate, set header, prepare buffers)
    int rc_deflate_init = deflateInit2_(&dstrm, 6, 8, 31, 8, 0, version, (int)sizeof(z_stream));
    int rc_set_header = deflateSetHeader(&dstrm, &head);
    dstrm.next_in = (Bytef *)payload;
    dstrm.avail_in = (uInt)payload_len;
    uLong bound = deflateBound(&dstrm, payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)(bound ? bound : 1);
    int rc_deflate = deflate(&dstrm, 0);
    uLong comp_size = dstrm.total_out;
    int rc_deflate_end = deflateEnd(&dstrm);

    // step 3: Core operations (write compressed data to gz files, initialize inflate, copy stream, inflate)
    gzFile gzf1 = gzopen("zlib_seq_out1.gz", "wb");
    gzFile gzf2 = gzopen64("zlib_seq_out2.gz", "wb");
    int rc_gzwrite1 = gzwrite(gzf1, comp_buf, (unsigned int)(comp_size ? comp_size : 0));
    int rc_gzwrite2 = gzwrite(gzf2, comp_buf, (unsigned int)(comp_size ? comp_size : 0));
    int rc_gzclose_w1 = gzclose_w(gzf1);
    int rc_gzclose_w2 = gzclose_w(gzf2);
    int rc_inflate_init = inflateInit2_(&istrm, 31, version, (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(comp_size ? comp_size : 0);
    Bytef *decomp_buf = new Bytef[(payload_len ? payload_len : 1)];
    istrm.next_out = decomp_buf;
    istrm.avail_out = (uInt)(payload_len ? payload_len : 1);
    int rc_inflate_copy = inflateCopy(&istrm_copy, &istrm);
    int rc_inflate = inflate(&istrm, 0);

    // step 4: Validate and cleanup
    uLong crc_orig = crc32(0UL, (const Bytef *)payload, (uInt)payload_len);
    uLong crc_decomp = crc32(0UL, decomp_buf, (uInt)(payload_len ? payload_len : 0));
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_inflate_end_copy = inflateEnd(&istrm_copy);
    delete [] comp_buf;
    delete [] decomp_buf;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_set_header;
    (void)rc_deflate;
    (void)rc_deflate_end;
    (void)rc_gzwrite1;
    (void)rc_gzwrite2;
    (void)rc_gzclose_w1;
    (void)rc_gzclose_w2;
    (void)rc_inflate_init;
    (void)rc_inflate_copy;
    (void)rc_inflate;
    (void)crc_orig;
    (void)crc_decomp;
    (void)rc_inflate_end;
    (void)rc_inflate_end_copy;
    // API sequence test completed successfully
    return 66;
}