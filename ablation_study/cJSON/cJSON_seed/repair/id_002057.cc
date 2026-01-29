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
//<ID> 2057
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
    cJSON_AddNumberToObject(root, "count", 0.0);
    cJSON_AddStringToObject(root, "label", "original");

    // step 2: Configure
    char json_buf[] = " { \"name\" : \"minified_name\" , \"val\" : 3.14 } ";
    cJSON_Minify(json_buf);
    cJSON *parsed = cJSON_Parse(json_buf);
    cJSON *name_item = cJSON_GetObjectItem(parsed, "name");
    const char *name_str = cJSON_GetStringValue(name_item);
    cJSON *name_entry = cJSON_CreateString(name_str);
    cJSON_AddItemToArray(items, name_entry);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", false_item);

    // step 3: Operate and Validate
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON *dup_items = cJSON_GetObjectItem(dup, "items");
    cJSON *first_dup = cJSON_GetArrayItem(dup_items, 0);
    cJSON_SetValuestring(first_dup, "duplicated_name");
    char *printed = cJSON_PrintUnformatted(dup);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed[0];
    buffer[1] = cJSON_GetStringValue(first_dup)[0];
    buffer[2] = name_str[0];
    buffer[3] = '\0';
    cJSON_free(printed);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}