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
//<ID> 1043
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

    // step 2: Configure
    cJSON_AddNullToObject(root, "maybe_null");
    cJSON_AddFalseToObject(root, "is_enabled");
    const char *version_str = cJSON_Version();
    cJSON_AddStringToObject(root, "version", version_str);
    cJSON_AddNumberToObject(root, "count", 2.0);
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(items, n1);
    cJSON_AddItemToArray(items, n2);

    // step 3: Operate and Validate
    cJSON *got_items = cJSON_GetObjectItem(root, "items");
    cJSON *got_maybe_null = cJSON_GetObjectItem(root, "maybe_null");
    cJSON *got_is_enabled = cJSON_GetObjectItem(root, "is_enabled");
    cJSON *got_version = cJSON_GetObjectItem(root, "version");
    int items_count = cJSON_GetArraySize(got_items);
    cJSON *first_item = cJSON_GetArrayItem(got_items, 0);
    double first_value = cJSON_GetNumberValue(first_item);
    cJSON_bool is_null = cJSON_IsNull(got_maybe_null);
    cJSON_bool is_false = cJSON_IsFalse(got_is_enabled);
    const char *ver_from_item = cJSON_GetStringValue(got_version);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = (char)('0' + (items_count & 7));
    ((char *)scratch)[1] = (char)((int)first_value % 10 + '0');
    ((char *)scratch)[2] = (char)(ver_from_item[0]);
    char *flat = cJSON_PrintUnformatted(root);
    cJSON_free(flat);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}