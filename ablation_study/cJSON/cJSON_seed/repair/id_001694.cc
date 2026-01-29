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
//<ID> 1694
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *name_item = cJSON_CreateString("sample_name");
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *it1 = cJSON_CreateString("one");
    cJSON *it2 = cJSON_CreateString("two");
    cJSON *it3 = cJSON_CreateString("three");
    cJSON_AddItemToArray(arr, it1);
    cJSON_AddItemToArray(arr, it2);
    cJSON_AddItemToArray(arr, it3);

    // step 2: Configure
    cJSON_AddStringToObject(meta, "status", "active");
    cJSON_AddNumberToObject(meta, "count", 3.0);
    cJSON_AddBoolToObject(meta, "enabled", 1);

    // step 3: Operate and Validate
    cJSON_bool has_meta = cJSON_HasObjectItem(root, "meta");
    cJSON *retr_name = cJSON_GetObjectItem(root, "name");
    const char *name_val = cJSON_GetStringValue(retr_name);
    cJSON *retr_items = cJSON_GetObjectItem(root, "items");
    cJSON_DeleteItemFromArray(retr_items, 0);
    cJSON *new_first = cJSON_GetArrayItem(retr_items, 0);
    const char *first_val = cJSON_GetStringValue(new_first);

    // step 4: Cleanup
    char *printed = cJSON_Print(root);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = name_val ? name_val[0] : 'Y';
    buffer[2] = first_val ? first_val[0] : 'Z';
    buffer[3] = (char)('0' + (int)has_meta);
    cJSON_free(printed);
    cJSON_Delete(root);
    (void)buffer;
    return 66; // API sequence test completed successfully
}