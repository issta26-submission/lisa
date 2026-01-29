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
//<ID> 581
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, initialize deflate, compute adler32, and compress with deflate
    const char src[] = "zlib api sequence payload: prepare -> deflate -> gzwrite -> inflate -> validate";
    const Bytef *source = (const Bytef *)src;
    uLong srcLen = (uLong)(sizeof(src) - 1);
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong ad = adler32(1L, source, (uInt)srcLen);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)srcLen;
    uLong outBound = deflateBound(&dstrm, srcLen);
    Bytef *outBuf = (Bytef *)malloc((size_t)outBound);
    memset(outBuf, 0, (size_t)outBound);
    dstrm.next_out = outBuf;
    dstrm.avail_out = (uInt)outBound;
    int rc_def_finish = deflate(&dstrm, 4);

    // step 2: Write compressed data to a gzFile and query direct mode
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, outBuf, (unsigned int)dstrm.total_out);
    int is_direct = gzdirect(gf);
    int rc_gzclose = gzclose(gf);

    // step 3: Initialize inflate, request gzip header parsing, perform inflate, and combine CRCs
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    gz_header header;
    memset(&header, 0, sizeof(gz_header));
    int rc_get_header = inflateGetHeader(&istrm, &header);
    istrm.next_in = outBuf;
    istrm.avail_in = (uInt)dstrm.total_out;
    uLong decBound = srcLen + 32;
    Bytef *decBuf = (Bytef *)malloc((size_t)decBound);
    memset(decBuf, 0, (size_t)decBound);
    istrm.next_out = decBuf;
    istrm.avail_out = (uInt)decBound;
    int rc_inflate = inflate(&istrm, 4);
    uInt split = (uInt)(srcLen / 2);
    uLong crc1 = crc32(0L, source, split);
    uLong crc2 = crc32(0L, source + split, (uInt)(srcLen - split));
    uLong crc_combined = crc32_combine(crc1, crc2, (off_t)(srcLen - split));

    // step 4: Cleanup streams and buffers
    int rc_def_end = deflateEnd(&dstrm);
    int rc_inf_end = inflateEnd(&istrm);
    free(outBuf);
    free(decBuf);
    (void)rc_def_init;
    (void)rc_def_finish;
    (void)rc_gzwrite;
    (void)is_direct;
    (void)rc_gzclose;
    (void)rc_inf_init;
    (void)rc_get_header;
    (void)rc_inflate;
    (void)ad;
    (void)crc_combined;
    (void)rc_def_end;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}