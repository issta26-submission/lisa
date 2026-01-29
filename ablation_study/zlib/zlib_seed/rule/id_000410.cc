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
//<ID> 410
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
static unsigned char *g_in_ptr;
static unsigned int g_in_len;
static unsigned char *g_out_ptr;
static unsigned int g_out_remaining;

static unsigned int z_in_cb(void *in_desc, unsigned char **buf) {
    /* Provide the compressed buffer to inflateBack */
    *buf = g_in_ptr;
    unsigned int ret = g_in_len;
    g_in_len = 0;
    return ret;
}

static int z_out_cb(void *out_desc, unsigned char *buf, unsigned int len) {
    /* Copy decompressed bytes into the output buffer */
    unsigned int to_copy = len;
    if (to_copy > g_out_remaining) to_copy = g_out_remaining;
    memcpy(g_out_ptr, buf, (size_t)to_copy);
    g_out_ptr += to_copy;
    g_out_remaining -= to_copy;
    return (int)to_copy;
}

int test_zlib_api_sequence() {
    z_stream defStrm;
    z_stream infStrm;
    z_stream infCopy;
    gz_header header;
    memset(&defStrm, 0, sizeof(defStrm));
    memset(&infStrm, 0, sizeof(infStrm));
    memset(&infCopy, 0, sizeof(infCopy));
    memset(&header, 0, sizeof(header));
    unsigned char input[256];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong checksum = adler32(0UL, (const Bytef *)input, (uInt)inputLen);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    Bytef * decompBuf = (Bytef *)malloc((size_t)inputLen);
    memset(decompBuf, 0, (size_t)inputLen);

    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    defStrm.next_in = (Bytef *)input;
    defStrm.avail_in = (uInt)inputLen;
    defStrm.next_out = compBuf;
    defStrm.avail_out = (uInt)bound;
    defStrm.adler = checksum;
    /* Finish the compression so total_out reflects produced data */
    deflate(&defStrm, 4);

    inflateInit_(&infStrm, zlibVersion(), (int)sizeof(z_stream));
    infStrm.next_in = compBuf;
    infStrm.avail_in = (uInt)defStrm.total_out;
    infStrm.next_out = decompBuf;
    infStrm.avail_out = (uInt)inputLen;

    g_in_ptr = compBuf;
    g_in_len = (unsigned int)defStrm.total_out;
    g_out_ptr = decompBuf;
    g_out_remaining = (unsigned int)inputLen;

    inflateCopy(&infCopy, &infStrm);
    inflateGetHeader(&infStrm, &header);

    inflateBack(&infStrm, (in_func)z_in_cb, NULL, (out_func)z_out_cb, NULL);

    deflateEnd(&defStrm);
    inflateEnd(&infStrm);
    inflateEnd(&infCopy);
    free(compBuf);
    free(decompBuf);
    return 66;
}