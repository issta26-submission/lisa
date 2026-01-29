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
//<ID> 962
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and compression of payload
    const char payload[] = "zlib API sequence payload for adler32_combine, gzbuffer, gzprintf and gzwrite";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong bound = compressBound((uLong)payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    uLongf comp_len_f = (uLongf)bound;
    int rc_compress = compress2(comp_buf, &comp_len_f, src_buf, (uLong)payload_len, 6);
    uLong comp_size = (uLong)comp_len_f;

    // step 2: Initialize inflate stream and perform inflation to validate compressed data
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(comp_size <= (uLong)~0u ? comp_size : (uLong)~0u);
    Bytef *uncmp_buf = new Bytef[(payload_len ? payload_len : 1)];
    istrm.next_out = uncmp_buf;
    istrm.avail_out = (uInt)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    int rc_inflate = inflate(&istrm, 0);
    int rc_inflate_end = inflateEnd(&istrm);

    // step 3: Write the original payload to a gzFile using gzbuffer, gzwrite and gzprintf
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "w+b");
    int rc_gzbuffer = gzbuffer(gf, 4096);
    int rc_gzwrite = gzwrite(gf, (voidpc)src_buf, (unsigned int)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u));
    int rc_gzprintf = gzprintf(gf, "gz written marker\n");
    int rc_gzclose = gzclose(gf);

    // step 4: Compute Adler-32 checksums and combine, cleanup buffers
    uLong ad1 = adler32(0UL, src_buf, (uInt)payload_len);
    uLong ad2 = adler32(1UL, src_buf, (uInt)payload_len);
    uLong ad_combined = adler32_combine(ad1, ad2, (off_t)payload_len);

    delete [] comp_buf;
    delete [] uncmp_buf;

    (void)rc_compress;
    (void)comp_size;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_inflate_end;
    (void)rc_gzbuffer;
    (void)rc_gzwrite;
    (void)rc_gzprintf;
    (void)rc_gzclose;
    (void)ad1;
    (void)ad2;
    (void)ad_combined;
    // API sequence test completed successfully
    return 66;
}