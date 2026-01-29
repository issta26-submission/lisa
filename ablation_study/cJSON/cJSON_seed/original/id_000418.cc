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
//<ID> 418
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json_literal[] = "{\"arr\":[1,2,3],\"obj\":{\"key\":\"value\"}}";
    size_t json_len = sizeof(json_literal) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json_literal, json_len, &parse_end, 1);

    // step 2: Configure
    cJSON *arr = cJSON_GetObjectItemCaseSensitive(root, "arr");
    cJSON *second_item = cJSON_GetArrayItem(arr, 1);
    double second_val = cJSON_GetNumberValue(second_item);
    cJSON *newobj = cJSON_CreateObject();
    cJSON *label = cJSON_CreateString("inserted");
    cJSON_AddItemToObject(newobj, "label", label);

    // step 3: Operate
    cJSON *newnum = cJSON_CreateNumber(second_val * 10.0);
    cJSON_InsertItemInArray(arr, 1, newnum);
    cJSON *item_at_one = cJSON_GetArrayItem(arr, 1);
    cJSON_AddItemToObject(root, "newobj", newobj);

    // step 4: Validate and Cleanup
    cJSON *obj = cJSON_GetObjectItemCaseSensitive(root, "obj");
    cJSON *key = cJSON_GetObjectItemCaseSensitive(obj, "key");
    const char *keystr = cJSON_GetStringValue(key);
    double inserted_val = cJSON_GetNumberValue(item_at_one);
    (void)keystr;
    (void)inserted_val;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}