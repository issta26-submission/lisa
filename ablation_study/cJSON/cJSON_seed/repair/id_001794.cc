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
//<ID> 1794
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
    cJSON *enabled_true = cJSON_CreateTrue();
    cJSON_AddItemToObject(obj1, "enabled", enabled_true);
    cJSON_AddItemToArray(items, obj1);
    cJSON *obj2 = cJSON_CreateObject();
    cJSON_AddNumberToObject(obj2, "value", 314.0);
    cJSON_AddItemToArray(items, obj2);

    // step 2: Configure
    cJSON *dup_obj1 = cJSON_Duplicate(obj1, 1);
    cJSON_AddItemToArray(items, dup_obj1);
    cJSON *initial_flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", initial_flag);
    cJSON *replacement_flag = cJSON_CreateFalse();
    cJSON_ReplaceItemInObject(root, "flag", replacement_flag);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    int array_size = cJSON_GetArraySize(items);
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    cJSON *enabled_item = cJSON_GetObjectItem(first_item, "enabled");
    cJSON_bool enabled_is_true = cJSON_IsTrue(enabled_item);
    cJSON *second_item = cJSON_GetArrayItem(items, 1);
    cJSON *value_item = cJSON_GetObjectItem(second_item, "value");
    double value_val = cJSON_GetNumberValue(value_item);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = enabled_is_true ? '1' : '0';
    buffer[2] = (char)('0' + ((int)value_val % 10));
    buffer[3] = (char)('0' + (array_size % 10));

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}