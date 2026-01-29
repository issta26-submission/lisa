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
//<ID> 1264
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "Zlib API sequence payload";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    Bytef *buf = new Bytef[(size_t)srcLen];
    memset(buf, 0, (size_t)srcLen);
    memcpy(buf, payload, (size_t)srcLen);
    uInt half = (uInt)(srcLen / 2UL);
    uInt len1 = half;
    uInt len2 = (uInt)(srcLen - half);
    Bytef *seg1 = buf;
    Bytef *seg2 = buf + len1;
    uLong crc_initial = crc32(0UL, buf, (uInt)srcLen);
    uLong ad1 = adler32(1UL, seg1, len1);
    uLong ad2 = adler32(1UL, seg2, len2);

    // step 2: Setup (inflate stream and header buffer)
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = buf;
    istrm.avail_in = (uInt)srcLen;
    gz_header head;
    memset(&head, 0, (size_t)sizeof(gz_header));
    Bytef *nameBuf = new Bytef[16];
    memset(nameBuf, 0, 16);
    head.name = nameBuf;
    head.name_max = (uInt)16;
    int rc_getheader1 = inflateGetHeader(&istrm, &head);

    // step 3: Core operations (gzip I/O and checksum combination)
    gzFile gzf = gzopen("zlib_seq_output.gz", "wb");
    int rc_putc1 = gzputc(gzf, (int)payload[0]);
    int rc_putc2 = gzputc(gzf, (int)payload[1]);
    istrm.adler = crc_initial;
    uLong combined_adler = adler32_combine(ad1, ad2, (off_t)len2);
    uLong crc_after = crc32(crc_initial, buf, (uInt)srcLen);
    int rc_getheader2 = inflateGetHeader(&istrm, &head);

    // step 4: Cleanup and finalization
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_gzclose = gzclose(gzf);
    delete [] buf;
    delete [] nameBuf;
    (void)version;
    (void)rc_inflate_init;
    (void)rc_getheader1;
    (void)rc_putc1;
    (void)rc_putc2;
    (void)combined_adler;
    (void)crc_after;
    (void)rc_getheader2;
    (void)rc_inflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}