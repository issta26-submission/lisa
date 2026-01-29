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
//<ID> 994
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and Initialize
    const char payload[] = "zlib API sequence payload using gzprintf, gzwrite, inflatePrime and get_crc_table";
    const unsigned int payload_len = (unsigned int)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    const z_crc_t *crc_tbl = get_crc_table();
    unsigned long pseudo_crc = (unsigned long)crc_tbl[0] ^ (unsigned long)payload_len;

    // step 2: Configure gzFile and write data
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "w+b");
    int rc_gzprintf = gzprintf(gf, "prefix:%s", payload);
    int rc_gzwrite = gzwrite(gf, (voidpc)src_buf, (unsigned int)payload_len);

    // step 3: Use inflatePrime and attempt to inflate (operate)
    int rc_inflate_prime = inflatePrime(&istrm, 3, 1);
    istrm.next_in = (Bytef *)src_buf;
    istrm.avail_in = (uInt)payload_len;
    Bytef *uncmp_buf = new Bytef[(payload_len ? payload_len * 2u : 1u)];
    istrm.next_out = uncmp_buf;
    istrm.avail_out = (uInt)(payload_len * 2u);
    int rc_inflate = inflate(&istrm, 0);

    // step 4: Validate and Cleanup
    int rc_gzclose = gzclose(gf);
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] uncmp_buf;
    (void)rc_inflate_init;
    (void)pseudo_crc;
    (void)rc_gzprintf;
    (void)rc_gzwrite;
    (void)rc_inflate_prime;
    (void)rc_inflate;
    (void)rc_gzclose;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}