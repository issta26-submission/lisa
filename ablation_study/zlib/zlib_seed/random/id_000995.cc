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
//<ID> 995
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initial compression
    const char payload[] = "zlib API sequence payload exercising gzwrite, inflatePrime, get_crc_table and gzprintf";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong bound = compressBound(payload_len);
    uLongf comp_len_f = (uLongf)bound;
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    int rc_compress = compress2(comp_buf, &comp_len_f, src_buf, (uLong)payload_len, 6);

    // step 2: Initialize and configure inflate stream
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)comp_len_f;
    Bytef *uncmp_buf = new Bytef[(payload_len ? payload_len : 1)];
    istrm.next_out = uncmp_buf;
    istrm.avail_out = (uInt)payload_len;

    // step 3: Operate: prime inflate, perform inflate, then write gz file using gzwrite and gzprintf; also read CRC table
    int rc_inflate_prime = inflatePrime(&istrm, 0, 0);
    int rc_inflate = inflate(&istrm, 0);
    const z_crc_t *crc_table = get_crc_table();
    z_crc_t tbl0 = crc_table ? crc_table[0] : (z_crc_t)0;
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, (voidpc)src_buf, (unsigned int)payload_len);
    int rc_gzprintf = gzprintf(gf, "%s", payload);
    int rc_gzclose = gzclose(gf);

    // step 4: Validate and Cleanup
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] comp_buf;
    delete [] uncmp_buf;

    (void)rc_compress;
    (void)rc_inflate_init;
    (void)rc_inflate_prime;
    (void)rc_inflate;
    (void)tbl0;
    (void)rc_gzwrite;
    (void)rc_gzprintf;
    (void)rc_gzclose;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}