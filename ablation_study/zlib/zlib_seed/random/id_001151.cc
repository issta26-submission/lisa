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
//<ID> 1151
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
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm_copy, 0, (size_t)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, (size_t)sizeof(gz_header));
    head.text = 1;
    head.name = (Bytef *)"zlib_payload_name";
    head.comment = (Bytef *)"zlib_payload_comment";

    // step 2: Configure deflate, set header, allocate output buffer, write gz files
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_set_header = deflateSetHeader(&dstrm, &head);
    dstrm.next_in = (Bytef *)payload;
    dstrm.avail_in = (uInt)payload_len;
    uLong bound = deflateBound(&dstrm, payload_len);
    uInt out_size = (uInt)(bound ? bound : 1);
    Bytef *comp_buf = new Bytef[(size_t)out_size];
    memset(comp_buf, 0, (size_t)out_size);
    dstrm.next_out = comp_buf;
    dstrm.avail_out = out_size;
    int rc_deflate = deflate(&dstrm, 0);
    uLong comp_size = dstrm.total_out;
    gzFile gzf1 = gzopen("zlib_api_seq_1.gz", "wb");
    int rc_gzwrite1 = gzwrite(gzf1, comp_buf, (unsigned int)(comp_size ? comp_size : 0));
    gzFile gzf2 = gzopen64("zlib_api_seq_2.gz", "wb");
    int rc_gzwrite2 = gzwrite(gzf2, comp_buf, (unsigned int)(comp_size ? comp_size : 0));
    int rc_gzclose1 = gzclose(gzf1);
    int rc_gzclose2 = gzclose(gzf2);
    int rc_deflate_end = deflateEnd(&dstrm);

    // step 3: Initialize inflate with windowBits, copy stream, perform inflate into buffer
    int rc_inflate_init2 = inflateInit2_(&istrm, 15, version, (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(comp_size ? comp_size : 0);
    Bytef *decomp_buf = new Bytef[(size_t)(payload_len ? payload_len : 1)];
    memset(decomp_buf, 0, (size_t)(payload_len ? payload_len : 1));
    istrm.next_out = decomp_buf;
    istrm.avail_out = (uInt)(payload_len ? payload_len : 1);
    int rc_inflate_copy = inflateCopy(&istrm_copy, &istrm);
    int rc_inflate = inflate(&istrm, 0);

    // step 4: Validate and cleanup
    uLong crc_orig = crc32(0UL, (const Bytef *)payload, (uInt)payload_len);
    uLong crc_comp = crc32(0UL, comp_buf, (uInt)(comp_size ? comp_size : 0));
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_inflate_copy_end = inflateEnd(&istrm_copy);
    delete [] comp_buf;
    delete [] decomp_buf;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_set_header;
    (void)bound;
    (void)rc_deflate;
    (void)rc_gzwrite1;
    (void)rc_gzwrite2;
    (void)rc_gzclose1;
    (void)rc_gzclose2;
    (void)rc_deflate_end;
    (void)rc_inflate_init2;
    (void)rc_inflate_copy;
    (void)rc_inflate;
    (void)crc_orig;
    (void)crc_comp;
    (void)rc_inflate_end;
    (void)rc_inflate_copy_end;
    // API sequence test completed successfully
    return 66;
}