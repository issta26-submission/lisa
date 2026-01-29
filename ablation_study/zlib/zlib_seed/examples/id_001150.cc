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
//<ID> 1150
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "zlib API sequence payload demonstrating inflateInit2_, deflateSetHeader, inflateCopy, gzopen and gzopen64";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    z_stream dstrm;
    z_stream istrm;
    z_stream istrm_copy;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm_copy, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit2_(&istrm, 15, version, (int)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    gz_header gzhead;
    memset(&gzhead, 0, (size_t)sizeof(gz_header));
    gzhead.text = 1;
    int rc_set_header = deflateSetHeader(&dstrm, &gzhead);

    // step 2: Setup (prepare buffers and compress)
    dstrm.next_in = (Bytef *)payload;
    dstrm.avail_in = (uInt)payload_len;
    uLong bound = deflateBound(&dstrm, payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    memset(comp_buf, 0, (size_t)(bound ? bound : 1));
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)(bound ? bound : 1);
    int rc_deflate = deflate(&dstrm, 4);
    uLong comp_size = dstrm.total_out;

    // step 3: Core operations (open gz files, write, prepare inflate copy and inflate)
    gzFile gzf1 = gzopen("test_zlib_api.gz", "wb");
    gzFile gzf2 = gzopen64("test_zlib_api64.gz", "wb");
    int rc_gzwrite1 = gzwrite(gzf1, payload, (unsigned int)payload_len);
    int rc_gzwrite2 = gzwrite(gzf2, comp_buf, (unsigned int)(comp_size ? comp_size : 0));
    Bytef *out_buf = new Bytef[(payload_len ? payload_len : 1)];
    memset(out_buf, 0, (size_t)(payload_len ? payload_len : 1));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(comp_size ? comp_size : 0);
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)(payload_len ? payload_len : 1);
    int rc_inflate_copy = inflateCopy(&istrm_copy, &istrm);
    int rc_inflate = inflate(&istrm_copy, 0);
    uLong decomp_size = istrm_copy.total_out;

    // step 4: Validate and cleanup
    uLong crc_orig = crc32(0UL, (const Bytef *)payload, (uInt)payload_len);
    uLong crc_comp = crc32(0UL, comp_buf, (uInt)(comp_size ? comp_size : 0));
    int rc_inflate_end_copy = inflateEnd(&istrm_copy);
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_gzclose1 = gzclose(gzf1);
    int rc_gzclose2 = gzclose(gzf2);
    delete [] comp_buf;
    delete [] out_buf;
    (void)version;
    (void)rc_inflate_init;
    (void)rc_deflate_init;
    (void)rc_set_header;
    (void)rc_deflate;
    (void)bound;
    (void)comp_size;
    (void)rc_gzwrite1;
    (void)rc_gzwrite2;
    (void)rc_inflate_copy;
    (void)rc_inflate;
    (void)decomp_size;
    (void)crc_orig;
    (void)crc_comp;
    (void)rc_inflate_end_copy;
    (void)rc_inflate_end;
    (void)rc_deflate_end;
    (void)rc_gzclose1;
    (void)rc_gzclose2;
    // API sequence test completed successfully
    return 66;
}