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
//<ID> 727
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    double numbers[3] = {1.5, 2.5, 3.5};
    cJSON *double_arr = cJSON_CreateDoubleArray(numbers, 3);
    cJSON *str1 = cJSON_CreateString("first");
    cJSON *str2 = cJSON_CreateString("second");
    cJSON *flag = cJSON_CreateFalse();

    // step 2: Configure
    cJSON_AddItemToArray(items, str1);
    cJSON_AddItemToArray(items, str2);
    cJSON_AddItemToArray(items, double_arr);
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON_AddItemToObject(root, "title", cJSON_CreateString("demo"));

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_items = cJSON_GetObjectItemCaseSensitive(parsed, "items");
    cJSON *first_item = cJSON_GetArrayItem(parsed_items, 0);
    const char *first_value = cJSON_GetStringValue(first_item);
    cJSON *parsed_flag = cJSON_GetObjectItemCaseSensitive(parsed, "flag");
    cJSON_bool flag_is_false = cJSON_IsFalse(parsed_flag);
    cJSON *detached_flag = cJSON_DetachItemFromObjectCaseSensitive(parsed, "flag");
    cJSON_AddItemToArray(parsed_items, detached_flag);
    (void)version;
    (void)first_value;
    (void)flag_is_false;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}