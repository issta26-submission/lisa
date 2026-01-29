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
//<ID> 992
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initial compression (Initialize)
    const char payload[] = "zlib API sequence payload exercising inflatePrime, get_crc_table, gzprintf and gzwrite";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong bound = compressBound((uLong)payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    uLongf comp_len_f = (uLongf)bound;
    int rc_compress = compress2(comp_buf, &comp_len_f, src_buf, (uLong)payload_len, 6);

    // step 2: Initialize and configure inflate stream (Configure)
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)comp_len_f;
    Bytef *uncmp_buf = new Bytef[(payload_len ? payload_len : 1)];
    istrm.next_out = uncmp_buf;
    istrm.avail_out = (uInt)payload_len;
    int rc_inflate_prime = inflatePrime(&istrm, 1, 1);
    const z_crc_t *crc_table = get_crc_table();
    z_crc_t table0 = crc_table[0];

    // step 3: Operate: inflate compressed buffer and write output with gz APIs (Operate)
    int rc_inflate = inflate(&istrm, 0);
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzprintf = gzprintf(gf, "%s", (const char *)uncmp_buf);
    int rc_gzwrite = gzwrite(gf, (voidpc)uncmp_buf, (unsigned int)payload_len);
    int rc_gzclose = gzclose(gf);

    // step 4: Validate and Cleanup (Validate -> Cleanup)
    uLong crc_calc = crc32(0UL, uncmp_buf, (uInt)payload_len);
    (void)rc_compress;
    (void)rc_inflate_init;
    (void)rc_inflate_prime;
    (void)table0;
    (void)rc_inflate;
    (void)rc_gzprintf;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] comp_buf;
    delete [] uncmp_buf;
    (void)crc_calc;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}