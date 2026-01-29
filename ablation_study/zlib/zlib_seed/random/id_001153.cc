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
//<ID> 1153
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "zlib API sequence payload to exercise inflateInit2_/deflateSetHeader/inflateCopy/gzopen/gzopen64";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    z_stream istrm;
    z_stream istrm_copy;
    z_stream dstrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm_copy, 0, (size_t)sizeof(z_stream));
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    gz_header header;
    memset(&header, 0, (size_t)sizeof(gz_header));

    // step 2: Setup (initialize inflate with custom windowBits, initialize deflate and set header)
    int rc_inflate_init2 = inflateInit2_(&istrm, 15, version, (int)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    Bytef *name_buf = new Bytef[16];
    memset(name_buf, 0, (size_t)16);
    header.name = name_buf;
    header.name_max = (uInt)16;
    header.text = 1;
    int rc_deflate_set_header = deflateSetHeader(&dstrm, &header);

    // step 3: Core operations (deflate the payload, write compressed data to gz files, inflate and copy)
    dstrm.next_in = (Bytef *)payload;
    dstrm.avail_in = (uInt)payload_len;
    uLong bound = deflateBound(&dstrm, payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    memset(comp_buf, 0, (size_t)(bound ? bound : 1));
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)(bound ? bound : 1);
    int rc_deflate = deflate(&dstrm, 0);
    uLong comp_size = dstrm.total_out;
    gzFile gzf1 = gzopen("zlib_seq_test.gz", "wb");
    gzFile gzf2 = gzopen64("zlib_seq_test64.gz", "wb");
    int rc_gzwrite1 = gzwrite(gzf1, comp_buf, (unsigned int)(comp_size ? comp_size : 0));
    int rc_gzwrite2 = gzwrite(gzf2, comp_buf, (unsigned int)(comp_size ? comp_size : 0));

    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(comp_size ? comp_size : 0);
    Bytef *out_buf = new Bytef[(payload_len ? payload_len : 1)];
    memset(out_buf, 0, (size_t)(payload_len ? payload_len : 1));
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)(payload_len ? payload_len : 1);
    int rc_inflate = inflate(&istrm, 0);
    int rc_inflate_copy = inflateCopy(&istrm_copy, &istrm);

    // step 4: Validate and cleanup
    uLong checksum_orig = adler32_z(0UL, (const Bytef *)payload, (z_size_t)payload_len);
    uLong checksum_out = adler32_z(0UL, out_buf, (z_size_t)(payload_len ? payload_len : 0));
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_inflate_copy_end = inflateEnd(&istrm_copy);
    int rc_gzclose1 = gzclose(gzf1);
    int rc_gzclose2 = gzclose(gzf2);
    delete [] comp_buf;
    delete [] out_buf;
    delete [] name_buf;
    (void)version;
    (void)rc_inflate_init2;
    (void)rc_deflate_init;
    (void)rc_deflate_set_header;
    (void)rc_deflate;
    (void)bound;
    (void)comp_size;
    (void)rc_gzwrite1;
    (void)rc_gzwrite2;
    (void)rc_inflate;
    (void)rc_inflate_copy;
    (void)checksum_orig;
    (void)checksum_out;
    (void)rc_deflate_end;
    (void)rc_inflate_end;
    (void)rc_inflate_copy_end;
    (void)rc_gzclose1;
    (void)rc_gzclose2;
    // API sequence test completed successfully
    return 66;
}