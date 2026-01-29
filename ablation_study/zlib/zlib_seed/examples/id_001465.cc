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
//<ID> 1465
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    uLong compBound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)compBound];
    memset(compBuf, 0, (size_t)compBound);
    z_stream defstrm;
    memset(&defstrm, 0, sizeof(defstrm));
    z_stream infstrm;
    memset(&infstrm, 0, sizeof(infstrm));
    gz_header head;
    memset(&head, 0, sizeof(head));
    char namebuf[] = "test_name";
    head.name = (Bytef *)namebuf;
    head.name_max = (uInt)sizeof(namebuf);
    head.time = 0x5A5A5A5A;

    // step 2: Setup
    defstrm.next_in = src;
    defstrm.avail_in = (uInt)srcLen;
    defstrm.next_out = compBuf;
    defstrm.avail_out = (uInt)compBound;
    const char *version = zlibVersion();
    int rc_definit = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    int rc_inflateinit = inflateInit_(&infstrm, version, (int)sizeof(z_stream));

    // step 3: Core operations
    int rc_sethead = deflateSetHeader(&defstrm, &head);
    unsigned int pending = 0u;
    int bits = 0;
    int rc_pending = deflatePending(&defstrm, &pending, &bits);
    gzFile gzf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_ungetc = gzungetc((int)'Z', gzf);
    int rc_undermine = inflateUndermine(&infstrm, 1);

    // step 4: Cleanup
    int rc_defend = deflateEnd(&defstrm);
    int rc_inflend = inflateEnd(&infstrm);
    int rc_gzclose = gzclose(gzf);
    delete [] src;
    delete [] compBuf;
    (void)version;
    (void)rc_definit;
    (void)rc_inflateinit;
    (void)rc_sethead;
    (void)pending;
    (void)bits;
    (void)rc_pending;
    (void)rc_ungetc;
    (void)rc_undermine;
    (void)rc_defend;
    (void)rc_inflend;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}