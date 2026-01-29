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
//<ID> 997
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initial checksum using CRC table
    const char payload[] = "zlib payload for gzprintf and gzwrite with inflatePrime usage";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    const z_crc_t *crc_tab = get_crc_table();
    uLong initial_seed = (uLong)crc_tab[0];
    uLong crc = crc32(initial_seed, src_buf, (uInt)payload_len);

    // step 2: Initialize and configure inflate stream
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    int rc_inflate_prime = inflatePrime(&istrm, 3, 5);

    // step 3: Operate with gzFile: formatted write and raw write
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzprintf = gzprintf(gf, "Payload:%s", payload);
    int rc_gzwrite = gzwrite(gf, (voidpc)src_buf, (unsigned int)payload_len);
    int rc_gzflush = gzflush(gf, 1);
    int rc_gzclose = gzclose(gf);

    // step 4: Validate and cleanup
    int rc_inflate_end = inflateEnd(&istrm);

    (void)crc_tab;
    (void)initial_seed;
    (void)crc;
    (void)rc_inflate_init;
    (void)rc_inflate_prime;
    (void)rc_gzprintf;
    (void)rc_gzwrite;
    (void)rc_gzflush;
    (void)rc_gzclose;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}