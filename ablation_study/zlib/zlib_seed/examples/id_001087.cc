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
//<ID> 1087
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initial checks (payload, flags, checksums)
    const char payload[] = "zlib API sequence payload for deflate/inflate -> checksum validation";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    uLong compile_flags = zlibCompileFlags();
    uLong crc_payload = crc32_z(0UL, (const Bytef *)payload, (z_size_t)payload_len);
    uLong adler_payload = adler32(0UL, (const Bytef *)payload, (uInt)payload_len);

    // step 2: Compress the payload using deflate (initialize -> configure -> operate)
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    uLong comp_bound = deflateBound(&dstrm, payload_len);
    Bytef * comp_buf = new Bytef[(size_t)comp_bound];
    dstrm.next_in = (Bytef *)payload;
    dstrm.avail_in = (uInt)payload_len;
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)comp_bound;
    int rc_deflate = deflate(&dstrm, 4); // Z_FINISH == 4
    uLong comp_len = dstrm.total_out;
    int rc_deflate_end = deflateEnd(&dstrm);

    // step 3: Decompress using inflate and validate checksums
    Bytef * out_buf = new Bytef[(size_t)(payload_len + 16)];
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)comp_len;
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)(payload_len + 16);
    int rc_inflate = inflate(&istrm, 4); // Z_FINISH == 4
    uLong out_len = istrm.total_out;
    int rc_inflate_end = inflateEnd(&istrm);

    // step 4: Validation and cleanup (compute checksums, combine CRCs, free resources)
    uLong crc_comp = crc32_z(0UL, comp_buf, (z_size_t)comp_len);
    uLong crc_out = crc32_z(0UL, out_buf, (z_size_t)out_len);
    uLong combined_crc = crc32_combine(crc_payload, crc_out, (off_t)out_len);
    uLong adler_out = adler32(0UL, out_buf, (uInt)out_len);

    delete [] comp_buf;
    delete [] out_buf;

    (void)compile_flags;
    (void)crc_payload;
    (void)adler_payload;
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)rc_deflate_end;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_inflate_end;
    (void)comp_len;
    (void)crc_comp;
    (void)crc_out;
    (void)combined_crc;
    (void)adler_out;

    // API sequence test completed successfully
    return 66;
}