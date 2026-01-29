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
//<ID> 109
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "name", "synth");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *flag = cJSON_AddTrueToObject(root, "enabled");
    cJSON *num = cJSON_CreateNumber(1.234);
    cJSON_AddItemToArray(arr, num);
    cJSON *str = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(arr, str);
    cJSON *ins = cJSON_CreateString("inserted");
    cJSON_InsertItemInArray(arr, 1, ins);

    // step 3: Operate and Validate
    char *printed = cJSON_Print(root);
    int arr_size = cJSON_GetArraySize(arr);
    cJSON_Minify(printed);
    cJSON *item = cJSON_GetArrayItem(arr, 1);
    const char *item_str = cJSON_GetStringValue(item);
    char buffer[128];
    memset(buffer, 0, sizeof(buffer));
    int summary = arr_size + (printed ? (int)printed[0] : 0) + (item_str ? (int)item_str[0] : 0) + (int)buffer[0] + (int)flag->type;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}