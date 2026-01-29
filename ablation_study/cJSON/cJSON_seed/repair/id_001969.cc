#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 1969
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *verstr = cJSON_CreateString(cJSON_Version());
    cJSON_AddItemToObject(meta, "version", verstr);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(meta, "ok", flag);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *status = cJSON_CreateString("ready");
    cJSON_AddItemToObject(root, "status", status);

    // step 2: Configure
    cJSON *details = cJSON_CreateObject();
    cJSON *note = cJSON_CreateString("configured");
    cJSON_AddItemToObject(details, "note", note);
    cJSON_AddItemToObject(root, "details", details);
    cJSON *marker = cJSON_CreateString("marker_value");
    cJSON_AddItemToObject(details, "marker", marker);

    // step 3: Operate and Validate
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *got_ver = cJSON_GetObjectItem(got_meta, "version");
    const char *version_value = cJSON_GetStringValue(got_ver);
    const int buflen = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    cJSON_bool flag_is_false = cJSON_IsFalse(flag);
    buffer[0] = (char)('0' + (printed_ok % 10));
    buffer[1] = (char)('0' + (flag_is_false % 10));
    buffer[2] = '\0';
    (void)version_value;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}