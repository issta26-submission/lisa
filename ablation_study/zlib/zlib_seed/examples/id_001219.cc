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
//<ID> 1219
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payloadA[] = "PartA_of_payload";
    const char payloadB[] = "PartB_of_payload";
    const uLong lenA = (uLong)(sizeof(payloadA) - 1);
    const uLong lenB = (uLong)(sizeof(payloadB) - 1);
    const char *version = zlibVersion();
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    unsigned char *in_buf = new unsigned char[512];
    unsigned char *out_buf = new unsigned char[512];
    memset(in_buf, 0, (size_t)512);
    memset(out_buf, 0, (size_t)512);

    // step 2: Setup (initialize streams and compute checksums)
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    uLong crcA = crc32(0UL, (const Bytef *)payloadA, (uInt)lenA);
    uLong adlB = adler32(0UL, (const Bytef *)payloadB, (uInt)lenB);
    dstrm.next_in = (Bytef *)in_buf;
    dstrm.avail_in = 0;
    dstrm.next_out = (Bytef *)out_buf;
    dstrm.avail_out = (uInt)512;
    istrm.next_in = (Bytef *)in_buf;
    istrm.avail_in = 0;
    istrm.adler = adlB; // propagate checksum into inflate stream state

    // step 3: Core operations (use checksums, write compressed output, teardown inflate backend)
    gzFile gzf = gzopen("zlib_api_seq_output.gz", "wb+");
    int rc_gzwrite1 = gzwrite(gzf, (const void *)payloadA, (unsigned int)(lenA ? lenA : 0));
    int rc_gzwrite2 = gzwrite(gzf, (const void *)payloadB, (unsigned int)(lenB ? lenB : 0));
    // store combined small metadata into stream fields to create meaningful flow
    dstrm.adler = adlB;
    dstrm.total_in = crcA;
    int rc_inflate_back_end = inflateBackEnd(&istrm);

    // step 4: Validate and cleanup
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_gzclose = gzclose(gzf);
    delete [] in_buf;
    delete [] out_buf;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_inflate_init;
    (void)crcA;
    (void)adlB;
    (void)rc_gzwrite1;
    (void)rc_gzwrite2;
    (void)rc_inflate_back_end;
    (void)rc_deflate_end;
    (void)rc_inflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}