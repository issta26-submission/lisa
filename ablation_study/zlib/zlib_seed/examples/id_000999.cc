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
//<ID> 999
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and Initialize
    const char payload[] = "zlib API sequence payload exercising inflatePrime, get_crc_table, gzprintf and gzwrite";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong bound = compressBound((uLong)payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    uLongf comp_len_f = (uLongf)bound;
    int rc_compress = compress2(comp_buf, &comp_len_f, src_buf, (uLong)payload_len, 6);
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure (prepare stream and CRC table)
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)comp_len_f;
    const z_crc_t *crc_tbl = get_crc_table();
    z_crc_t sample_crc = crc_tbl ? crc_tbl[0] : (z_crc_t)0;
    int rc_inflate_prime = inflatePrime(&istrm, 3, 5);

    // step 3: Operate (inflate and write gz file)
    Bytef *uncmp_buf = new Bytef[(payload_len ? payload_len : 1)];
    istrm.next_out = uncmp_buf;
    istrm.avail_out = (uInt)payload_len;
    int rc_inflate = inflate(&istrm, 0);
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzprintf = gzprintf(gw, "%s %u", payload, (unsigned)sample_crc);
    int rc_gzwrite = gzwrite(gw, (voidpc)src_buf, (unsigned)payload_len);
    int rc_gzclose = gzclose(gw);

    // step 4: Validate and Cleanup
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] comp_buf;
    delete [] uncmp_buf;
    (void)rc_compress;
    (void)rc_inflate_init;
    (void)rc_inflate_prime;
    (void)rc_inflate;
    (void)rc_gzprintf;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}