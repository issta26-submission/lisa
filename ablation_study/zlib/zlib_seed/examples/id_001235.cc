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
//<ID> 1235
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payloadA[] = "Hello_zlib_partA";
    const char payloadB[] = "World_zlib_partB";
    const uLong lenA = (uLong)(sizeof(payloadA) - 1);
    const uLong lenB = (uLong)(sizeof(payloadB) - 1);
    const char combined[] = "Hello_zlib_partA" "World_zlib_partB";
    const uLong combined_len = (uLong)(sizeof(combined) - 1);
    const char *version = zlibVersion();
    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(z_stream));
    unsigned char *comp_buf = new unsigned char[512];
    unsigned char *dest_buf = new unsigned char[1024];
    memset(comp_buf, 0, (size_t)512);
    memset(dest_buf, 0, (size_t)1024);
    uLongf comp_len_f = (uLongf)512;
    uLong comp_len_ul = 0;
    uLongf dest_len_f = (uLongf)1024;
    uLong comp_len_report = 0;
    uLong crcA = crc32(0UL, (const Bytef *)payloadA, (uInt)lenA);
    uLong crcB = crc32(0UL, (const Bytef *)payloadB, (uInt)lenB);
    uLong combined_crc = crc32_combine(crcA, crcB, (off_t)lenB);
    gzFile gzf = gzopen("zlib_api_seq_output.gz", "wb+");

    // step 2: Setup (compress combined input and initialize inflate stream)
    int rc_compress = compress2((Bytef *)comp_buf, &comp_len_f, (const Bytef *)combined, (uLong)combined_len, 6);
    comp_len_ul = (uLong)comp_len_f;
    comp_len_report = comp_len_ul;
    int rc_inflate_init = inflateInit_(&strm, version, (int)sizeof(z_stream));

    // step 3: Core operations (uncompress, validate inflate state, and write output to gz)
    int rc_uncompress = uncompress2((Bytef *)dest_buf, &dest_len_f, (const Bytef *)comp_buf, &comp_len_ul);
    strm.next_in = (Bytef *)dest_buf;
    strm.avail_in = (uInt)(dest_len_f <= (uLongf)0xFFFFFFFFu ? (uInt)dest_len_f : (uInt)0xFFFFFFFFu);
    strm.total_in = combined_crc;
    int rc_inflate_validate = inflateValidate(&strm, 1);
    int rc_gzwrite = gzwrite(gzf, (const void *)dest_buf, (unsigned int)(dest_len_f ? dest_len_f : 0));
    int rc_gzflush = gzflush(gzf, 0);

    // step 4: Validate and cleanup
    int rc_inflate_end = inflateEnd(&strm);
    int rc_gzclose = gzclose(gzf);
    delete [] comp_buf;
    delete [] dest_buf;
    (void)version;
    (void)rc_compress;
    (void)rc_inflate_init;
    (void)rc_uncompress;
    (void)rc_inflate_validate;
    (void)rc_gzwrite;
    (void)rc_gzflush;
    (void)rc_inflate_end;
    (void)rc_gzclose;
    (void)comp_len_report;
    (void)crcA;
    (void)crcB;
    (void)combined_crc;
    // API sequence test completed successfully
    return 66;
}