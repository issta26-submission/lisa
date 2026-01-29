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
//<ID> 1236
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload1[] = "Payload_one_for_zlib";
    const char payload2[] = "Payload_two_for_zlib";
    const uLong len1 = (uLong)(sizeof(payload1) - 1);
    const uLong len2 = (uLong)(sizeof(payload2) - 1);
    const char *version = zlibVersion();
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    unsigned char *comp_buf = new unsigned char[1024];
    unsigned char *uncomp_buf = new unsigned char[2048];
    memset(comp_buf, 0, (size_t)1024);
    memset(uncomp_buf, 0, (size_t)2048);
    uLongf comp_buf_size = (uLongf)1024;
    uLongf uncompressed_expected = (uLongf)2048;
    uLong source_len_for_uncompress = 0;

    // step 2: Setup (compress first payload and initialize inflate stream)
    int rc_compress = compress2((Bytef *)comp_buf, &comp_buf_size, (const Bytef *)payload1, (uLong)len1, 6);
    source_len_for_uncompress = (uLong)comp_buf_size;
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = (Bytef *)comp_buf;
    istrm.avail_in = (uInt)(comp_buf_size <= (uLong)0xFFFFFFFF ? (uInt)comp_buf_size : (uInt)0xFFFFFFFF);
    istrm.next_out = (Bytef *)uncomp_buf;
    istrm.avail_out = (uInt)uncompressed_expected;

    // step 3: Core operations (uncompress, validate inflate state, compute and combine CRCs, write gz file)
    int rc_uncompress = uncompress2((Bytef *)uncomp_buf, &uncompressed_expected, (const Bytef *)comp_buf, &source_len_for_uncompress);
    int rc_inflate_validate = inflateValidate(&istrm, 1);
    uLong crc1 = crc32(0UL, (const Bytef *)payload1, (uInt)len1);
    uLong crc2 = crc32(0UL, (const Bytef *)payload2, (uInt)len2);
    uLong combined_crc = crc32_combine(crc1, crc2, (off_t)len2);
    gzFile gzf = gzopen("zlib_api_sequence_out.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (const void *)payload2, (unsigned int)(len2 ? len2 : 0));

    // step 4: Cleanup (finalize inflate, close gz, free buffers)
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_gzclose = gzclose(gzf);
    delete [] comp_buf;
    delete [] uncomp_buf;
    (void)version;
    (void)rc_compress;
    (void)rc_inflate_init;
    (void)rc_uncompress;
    (void)rc_inflate_validate;
    (void)crc1;
    (void)crc2;
    (void)combined_crc;
    (void)rc_gzwrite;
    (void)rc_inflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}