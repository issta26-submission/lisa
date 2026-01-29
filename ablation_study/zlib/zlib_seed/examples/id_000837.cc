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
//<ID> 837
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

/* Minimal in_func: provide no input (EOF) */
static unsigned int fuzz_in(void *in_desc, unsigned char **buf) {
    (void)in_desc;
    *buf = NULL;
    return 0;
}

/* Minimal out_func: accept output but do nothing */
static int fuzz_out(void *out_desc, unsigned char *buf, unsigned int len) {
    (void)out_desc;
    (void)buf;
    (void)len;
    return 0;
}

int test_zlib_api_sequence() {
    /* step 1: Declarations and initialization */
    const char src[] = "zlib payload: gzwrite -> gzoffset64 -> inflateBack -> crc32";
    const unsigned int srcLen = (unsigned int)(sizeof(src) - 1);
    const Bytef *source = (const Bytef *)src;
    uLong initial_crc = crc32(0L, source, (uInt)srcLen);
    const char *version = zlibVersion();

    /* step 2: Write compressed data to a gzip file and inspect offset */
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gz_write = gzwrite(gf, source, (unsigned int)srcLen);
    off64_t file_offset = gzoffset64(gf);
    int rc_gz_close = gzclose(gf);

    /* step 3: Initialize inflateBack stream, perform a no-op inflateBack call, and finalize */
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int rc_inflate_back_init = inflateBackInit_(&strm, 15, window, version, (int)sizeof(z_stream));
    int rc_inflate_back = inflateBack(&strm, fuzz_in, NULL, fuzz_out, NULL);
    int rc_inflate_back_end = inflateBackEnd(&strm);

    /* step 4: Validation and cleanup (suppress unused warnings) */
    (void)initial_crc;
    (void)version;
    (void)rc_gz_write;
    (void)file_offset;
    (void)rc_gz_close;
    (void)rc_inflate_back_init;
    (void)rc_inflate_back;
    (void)rc_inflate_back_end;
    free(window);

    /* API sequence test completed successfully */
    return 66;
}