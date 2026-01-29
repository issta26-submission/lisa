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
//<ID> 1959
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *first = cJSON_CreateObject();
    cJSON *name1 = cJSON_CreateString("first");
    cJSON_AddItemToObject(first, "name", name1);
    cJSON_AddItemToArray(items, first);
    cJSON *second = cJSON_CreateObject();
    cJSON_AddItemToArray(items, second);
    cJSON *val = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(second, "value", val);
    cJSON *raw = cJSON_CreateRaw("rawdata");
    cJSON_AddItemToArray(items, raw);

    // step 2: Configure
    const char *ver = cJSON_Version();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *verstr = cJSON_CreateString(ver);
    cJSON_AddItemToObject(meta, "version", verstr);
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON *s1 = cJSON_CreateString("a");
    cJSON_AddItemToArray(arr, s1);

    // step 3: Operate and Validate
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *got_ver = cJSON_GetObjectItem(got_meta, "version");
    cJSON_bool meta_is_obj = cJSON_IsObject(got_meta);
    cJSON_bool raw_is_invalid = cJSON_IsInvalid(raw);
    const int buflen = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_PrintPreallocated(root, buffer, buflen, 0);
    buffer[0] = (char)('0' + (meta_is_obj % 10));
    buffer[1] = (char)('0' + (raw_is_invalid % 10));
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}