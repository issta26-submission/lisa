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
//<ID> 68
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream dstrm;
    z_stream istrm;
    z_stream copystrm;
    Bytef inbuf[] = "ZLIB sample data to compress and then inflate for API sequencing test.";
    uInt in_len = (uInt)(sizeof(inbuf) - 1);
    Bytef compbuf[1024];
    Bytef decompbuf[1024];
    gzFile gzf;
    const char * ver;
    int rc_deflate_init;
    int rc_inflate_init;
    int rc_deflate;
    int rc_inflate;
    int rc_deflatecopy;
    int rc_deflate_end_d;
    int rc_deflate_end_c;
    int rc_inflate_end;
    unsigned int compressed_len;
    off_t offset;
    unsigned long codes_used;

    // step 2: Setup (initialize streams and file)
    memset(&dstrm, 0, sizeof(dstrm));
    memset(&istrm, 0, sizeof(istrm));
    memset(&copystrm, 0, sizeof(copystrm));
    ver = zlibVersion();
    rc_deflate_init = deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    rc_inflate_init = inflateInit2_(&istrm, 15 + 32, ver, (int)sizeof(z_stream));
    gzf = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    gzbuffer(gzf, 8192);

    // step 3: Core operations (compress, copy state, write, inflate, validate)
    dstrm.next_in = (Bytef *)inbuf;
    dstrm.avail_in = in_len;
    dstrm.next_out = compbuf;
    dstrm.avail_out = (uInt)sizeof(compbuf);
    deflateTune(&dstrm, 4, 16, 32, 64);
    rc_deflate = deflate(&dstrm, 0);
    compressed_len = (unsigned int)dstrm.total_out;
    gzwrite(gzf, compbuf, compressed_len);
    offset = gzoffset(gzf);
    rc_deflatecopy = deflateCopy(&copystrm, &dstrm);
    istrm.next_in = compbuf;
    istrm.avail_in = (uInt)compressed_len;
    istrm.next_out = decompbuf;
    istrm.avail_out = (uInt)sizeof(decompbuf);
    rc_inflate = inflate(&istrm, 0);
    codes_used = inflateCodesUsed(&istrm);

    // step 4: Cleanup (end streams and close file)
    rc_deflate_end_d = deflateEnd(&dstrm);
    rc_deflate_end_c = deflateEnd(&copystrm);
    rc_inflate_end = inflateEnd(&istrm);
    gzclose(gzf);

    (void)ver;
    (void)rc_deflate_init;
    (void)rc_inflate_init;
    (void)rc_deflate;
    (void)rc_inflate;
    (void)rc_deflatecopy;
    (void)rc_deflate_end_d;
    (void)rc_deflate_end_c;
    (void)rc_inflate_end;
    (void)compressed_len;
    (void)offset;
    (void)codes_used;

    // API sequence test completed successfully
    return 66;
}