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
//<ID> 1154
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "zlib API sequence payload for deflate/inflate/gz usage";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    z_stream dstrm;
    z_stream istrm;
    z_stream istrm_copy;
    gz_header gzhead;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm_copy, 0, (size_t)sizeof(z_stream));
    memset(&gzhead, 0, (size_t)sizeof(gz_header));
    int rc_deflate_init = deflateInit2_(&dstrm, 6, 8, 15 + 16, 8, 0, version, (int)sizeof(z_stream));
    int rc_inflate_init = inflateInit2_(&istrm, 15 + 16, version, (int)sizeof(z_stream));

    // step 2: Configure (gzip header, deflate buffers)
    gzhead.text = 0;
    gzhead.name = (Bytef *)"payload_name";
    gzhead.name_max = (uInt)strlen((const char *)gzhead.name);
    gzhead.comment = (Bytef *)"payload_comment";
    gzhead.comm_max = (uInt)strlen((const char *)gzhead.comment);
    int rc_set_header = deflateSetHeader(&dstrm, &gzhead);
    dstrm.next_in = (Bytef *)payload;
    dstrm.avail_in = (uInt)payload_len;
    uLong bound = deflateBound(&dstrm, payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)(bound ? bound : 1);

    // step 3: Core operations (deflate, write gz files, prepare inflate copy and inflate)
    int rc_deflate = deflate(&dstrm, 4);
    uLong comp_size = dstrm.total_out;
    gzFile gzf1 = gzopen("zlib_api_sequence_test1.gz", "wb");
    gzFile gzf2 = gzopen64("zlib_api_sequence_test2.gz", "wb");
    int rc_gzwrite1 = gzwrite(gzf1, comp_buf, (unsigned int)(comp_size ? comp_size : 0));
    int rc_gzwrite2 = gzwrite(gzf2, comp_buf, (unsigned int)(comp_size ? comp_size : 0));
    int rc_inflate_copy = inflateCopy(&istrm_copy, &istrm);
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(comp_size ? comp_size : 0);
    Bytef *out_buf = new Bytef[(payload_len ? payload_len : 1)];
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)(payload_len ? payload_len : 1);
    istrm_copy.next_in = comp_buf;
    istrm_copy.avail_in = (uInt)(comp_size ? comp_size : 0);
    istrm_copy.next_out = out_buf;
    istrm_copy.avail_out = (uInt)(payload_len ? payload_len : 1);
    int rc_inflate = inflate(&istrm, 4);

    // step 4: Validate and cleanup
    uLong crc_orig = crc32(0UL, (const Bytef *)payload, (uInt)payload_len);
    uLong crc_decomp = crc32(0UL, out_buf, (uInt)payload_len);
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_inflate_copy_end = inflateEnd(&istrm_copy);
    int rc_gzclose1 = gzclose_w(gzf1);
    int rc_gzclose2 = gzclose_w(gzf2);
    delete [] comp_buf;
    delete [] out_buf;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_inflate_init;
    (void)rc_set_header;
    (void)rc_deflate;
    (void)bound;
    (void)rc_gzwrite1;
    (void)rc_gzwrite2;
    (void)rc_inflate_copy;
    (void)rc_inflate;
    (void)crc_orig;
    (void)crc_decomp;
    (void)rc_deflate_end;
    (void)rc_inflate_end;
    (void)rc_inflate_copy_end;
    (void)rc_gzclose1;
    (void)rc_gzclose2;
    // API sequence test completed successfully
    return 66;
}