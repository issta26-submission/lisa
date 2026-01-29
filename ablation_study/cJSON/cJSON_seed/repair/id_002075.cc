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
//<ID> 2075
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name_item = cJSON_CreateString("unit_test");
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON *count_item = cJSON_CreateNumber(2026.0);
    cJSON_AddItemToObject(root, "count", count_item);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(meta, "version", cJSON_CreateString("v1.0"));
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON *changed = cJSON_Duplicate(root, 1);
    cJSON_ReplaceItemInObject(changed, "count", cJSON_CreateNumber(3030.0));

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    const char *name_str = cJSON_GetStringValue(cJSON_GetObjectItem(root, "name"));
    double count_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "count"));
    cJSON_bool equal_orig_dup = cJSON_Compare(root, dup, 1);
    cJSON_bool equal_orig_changed = cJSON_Compare(root, changed, 1);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed ? printed[0] : '\0';
    buffer[1] = name_str ? name_str[0] : '\0';
    buffer[2] = (char)('0' + ((int)count_val % 10));
    buffer[3] = (char)('0' + (int)(equal_orig_dup != 0));
    buffer[4] = (char)('0' + (int)(equal_orig_changed != 0));
    buffer[5] = '\0';
    cJSON_free(printed);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(dup);
    cJSON_Delete(changed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}