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
//<ID> 1156
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "zlib API sequence payload for inflate/deflate/gz usage";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    z_stream defstrm;
    z_stream istrm;
    z_stream copystrm;
    memset(&defstrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    memset(&copystrm, 0, (size_t)sizeof(z_stream));
    gz_header header;
    memset(&header, 0, (size_t)sizeof(gz_header));
    header.name = (Bytef *)"zlib-name";
    header.comment = (Bytef *)"zlib-comment";

    // step 2: Setup (initialize deflate with gzip wrapper, set header, prepare buffers)
    int rc_deflate_init = deflateInit2_(&defstrm, 6, 8, 31, 8, 0, version, (int)sizeof(z_stream));
    int rc_set_header = deflateSetHeader(&defstrm, &header);
    defstrm.next_in = (Bytef *)payload;
    defstrm.avail_in = (uInt)payload_len;
    uLong bound = deflateBound(&defstrm, payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    memset(comp_buf, 0, (size_t)(bound ? bound : 1));
    defstrm.next_out = comp_buf;
    defstrm.avail_out = (uInt)(bound ? bound : 1);

    // step 3: Core operations (deflate -> write gz files -> initialize inflate and copy)
    int rc_deflate = deflate(&defstrm, 0);
    uLong comp_size = defstrm.total_out;
    gzFile gz1 = gzopen("zlib_api_sequence_test.gz", "wb");
    gzFile gz2 = gzopen64("zlib_api_sequence_test64.gz", "wb");
    int rc_gzwrite1 = gzwrite(gz1, comp_buf, (unsigned int)(comp_size ? comp_size : 0));
    int rc_gzwrite2 = gzwrite(gz2, comp_buf, (unsigned int)(comp_size ? comp_size : 0));
    int rc_inflate_init = inflateInit2_(&istrm, 31, version, (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(comp_size ? comp_size : 0);
    Bytef *out_buf1 = new Bytef[(payload_len ? payload_len : 1)];
    Bytef *out_buf2 = new Bytef[(payload_len ? payload_len : 1)];
    istrm.next_out = out_buf1;
    istrm.avail_out = (uInt)(payload_len ? payload_len : 1);
    int rc_inflate = inflate(&istrm, 0);
    int rc_inflate_copy = inflateCopy(&copystrm, &istrm);
    copystrm.next_out = out_buf2;
    copystrm.avail_out = (uInt)(payload_len ? payload_len : 1);
    int rc_inflate_copy_run = inflate(&copystrm, 0);

    // step 4: Validate and cleanup
    uLong crc_orig = crc32(0UL, (const Bytef *)payload, (uInt)payload_len);
    uLong crc_decomp1 = crc32(0UL, out_buf1, (uInt)(payload_len ? payload_len : 0));
    uLong crc_decomp2 = crc32(0UL, out_buf2, (uInt)(payload_len ? payload_len : 0));
    int rc_deflate_end = deflateEnd(&defstrm);
    int rc_inflate_end1 = inflateEnd(&istrm);
    int rc_inflate_end2 = inflateEnd(&copystrm);
    int rc_gzclose1 = gzclose(gz1);
    int rc_gzclose2 = gzclose(gz2);
    delete [] comp_buf;
    delete [] out_buf1;
    delete [] out_buf2;
    (void)rc_deflate_init;
    (void)rc_set_header;
    (void)rc_deflate;
    (void)rc_gzwrite1;
    (void)rc_gzwrite2;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_inflate_copy;
    (void)rc_inflate_copy_run;
    (void)crc_orig;
    (void)crc_decomp1;
    (void)crc_decomp2;
    (void)rc_deflate_end;
    (void)rc_inflate_end1;
    (void)rc_inflate_end2;
    (void)rc_gzclose1;
    (void)rc_gzclose2;
    (void)bound;
    (void)version;
    // API sequence test completed successfully
    return 66;
}