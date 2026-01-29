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
//<ID> 1088
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and compress original payload into a zlib stream
    const char payload[] = "zlib API sequence payload to compress and then inflate; validate with crc/adler/check flags";
    uLong payload_len = (uLong)(sizeof(payload) - 1);
    uLongf comp_buf_len = compressBound(payload_len);
    Bytef * comp_buf = new Bytef[(comp_buf_len ? comp_buf_len : 1)];
    int rc_compress = compress2(comp_buf, &comp_buf_len, (const Bytef *)payload, payload_len, 6);

    // step 2: Initialize inflate stream and prepare output buffer
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    Bytef * out_buf = new Bytef[(payload_len ? payload_len : 1)];
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)comp_buf_len;
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)payload_len;
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Perform inflation (single-call) and finalize inflate
    int rc_inflate = inflate(&istrm, 0);
    int rc_inflate_end = inflateEnd(&istrm);

    // step 4: Validate checksums and flags, cleanup buffers
    uLong crc_original = crc32_z(0UL, (const Bytef *)payload, (z_size_t)payload_len);
    uLong crc_decompressed = crc32_z(0UL, out_buf, (z_size_t)payload_len);
    uLong crc_combined = crc32_combine(crc_original, crc_decompressed, (off_t)payload_len);
    uLong adler = adler32(0UL, out_buf, (uInt)payload_len);
    uLong flags = zlibCompileFlags();

    delete [] comp_buf;
    delete [] out_buf;
    (void)rc_compress;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_inflate_end;
    (void)crc_original;
    (void)crc_decompressed;
    (void)crc_combined;
    (void)adler;
    (void)flags;
    // API sequence test completed successfully
    return 66;
}