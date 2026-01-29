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
//<ID> 1954
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
    cJSON *obj1 = cJSON_CreateObject();
    cJSON *name1 = cJSON_CreateString("first");
    cJSON_AddItemToObject(obj1, "name", name1);
    cJSON *val1 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(obj1, "value", val1);
    cJSON_AddItemToArray(items, obj1);
    cJSON *obj2 = cJSON_CreateObject();
    cJSON_AddItemToObject(obj2, "name", cJSON_CreateString("second"));
    cJSON_AddItemToObject(obj2, "value", cJSON_CreateNumber(3.14));
    cJSON_AddItemToArray(items, obj2);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    const char *ver = cJSON_Version();
    cJSON_AddItemToObject(meta, "version", cJSON_CreateString(ver));

    // step 3: Operate and Validate
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_PrintPreallocated(root, buffer, buflen, 0);
    cJSON *got_items = cJSON_GetObjectItem(root, "items");
    cJSON_bool items_is_object = cJSON_IsObject(got_items);
    cJSON_bool items_invalid = cJSON_IsInvalid(got_items);
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *got_version = cJSON_GetObjectItem(got_meta, "version");
    const char *verstr = cJSON_GetStringValue(got_version);
    buffer[0] = verstr ? verstr[0] : 'X';
    buffer[1] = (char)('0' + (items_is_object % 10));
    buffer[2] = (char)('0' + (items_invalid % 10));
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}