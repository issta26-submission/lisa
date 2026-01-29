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
//<ID> 1460
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib payload for header/pending/undermine/gzungetc";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *inBuf = new Bytef[(size_t)srcLen];
    memset(inBuf, 0, (size_t)srcLen);
    memcpy(inBuf, payload, (size_t)srcLen);
    const char *version = zlibVersion();

    z_stream defStrm;
    memset(&defStrm, 0, sizeof(defStrm));
    defStrm.next_in = inBuf;
    defStrm.avail_in = (uInt)srcLen;

    uLong outCap = compressBound(srcLen);
    Bytef *outBuf = new Bytef[(size_t)outCap];
    memset(outBuf, 0, (size_t)outCap);
    defStrm.next_out = outBuf;
    defStrm.avail_out = (uInt)outCap;

    gz_header header;
    memset(&header, 0, sizeof(header));
    header.os = 255;
    header.name = (Bytef *)"test-name";
    header.name_max = (uInt)(strlen("test-name") + 1);

    // step 2: Setup
    int rc_definit = deflateInit_(&defStrm, 6, version, (int)sizeof(z_stream));
    int rc_setheader = deflateSetHeader(&defStrm, &header);

    // step 3: Core operations
    int rc_deflate = deflate(&defStrm, 0);
    unsigned int pending_bytes = 0;
    int pending_bits = 0;
    int rc_pending = deflatePending(&defStrm, &pending_bytes, &pending_bits);

    z_stream infStrm;
    memset(&infStrm, 0, sizeof(infStrm));
    infStrm.next_in = outBuf;
    infStrm.avail_in = (uInt)defStrm.total_out;
    int rc_inflate_init = inflateInit_(&infStrm, version, (int)sizeof(z_stream));
    int rc_undermine = inflateUndermine(&infStrm, 1);

    gzFile gzf = gzopen("test_zlib_api_sequence_output.gz", "wb+");
    int rc_ungetc = gzungetc((int)payload[0], gzf);

    // step 4: Cleanup
    int rc_defend = deflateEnd(&defStrm);
    int rc_inflend = inflateEnd(&infStrm);
    int rc_gzclose = gzclose(gzf);
    delete [] inBuf;
    delete [] outBuf;
    (void)version;
    (void)rc_definit;
    (void)rc_setheader;
    (void)rc_deflate;
    (void)pending_bytes;
    (void)pending_bits;
    (void)rc_pending;
    (void)rc_inflate_init;
    (void)rc_undermine;
    (void)rc_ungetc;
    (void)rc_defend;
    (void)rc_inflend;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}