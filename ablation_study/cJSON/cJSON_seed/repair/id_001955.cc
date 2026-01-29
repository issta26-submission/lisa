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
//<ID> 1955
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
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *label = cJSON_CreateString("initial_label");
    cJSON_AddItemToObject(meta, "label", label);

    // step 2: Configure
    const char *version_str = cJSON_Version();
    cJSON *ver_item = cJSON_CreateString(version_str ? version_str : "unknown");
    cJSON_AddItemToObject(meta, "version", ver_item);
    cJSON *num1 = cJSON_CreateNumber(10.0);
    cJSON *num2 = cJSON_CreateNumber(20.5);
    cJSON_AddItemToArray(items, num1);
    cJSON_AddItemToArray(items, num2);
    cJSON *raw = cJSON_CreateRaw("{\"raw\":true}");
    cJSON_AddItemToArray(items, raw);

    // step 3: Operate and Validate
    const int buflen = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_PrintPreallocated(root, buffer, buflen, 0);
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *got_version = cJSON_GetObjectItem(got_meta, "version");
    cJSON_bool meta_is_obj = cJSON_IsObject(got_meta);
    cJSON *missing = cJSON_GetObjectItem(root, "does_not_exist");
    cJSON_bool missing_is_invalid = cJSON_IsInvalid(missing);
    buffer[0] = (char)('0' + (meta_is_obj % 10));
    buffer[1] = (char)('0' + (missing_is_invalid % 10));
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}