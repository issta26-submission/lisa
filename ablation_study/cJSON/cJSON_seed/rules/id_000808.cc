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
//<ID> 808
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *num0 = NULL;
    cJSON *num1 = NULL;
    cJSON *raw = NULL;
    cJSON *dup_arr = NULL;
    cJSON *item0 = NULL;
    cJSON *dup_item0 = NULL;
    cJSON *size_num = NULL;
    char *printed = NULL;
    int arr_size = 0;
    cJSON_bool items_equal = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(arr, num0);
    num1 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(arr, num1);
    raw = cJSON_CreateRaw("{\"meta\":true}");
    cJSON_AddItemToArray(arr, raw);
    cJSON_AddItemToObject(root, "data", arr);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    size_num = cJSON_CreateNumber((double)arr_size);
    cJSON_AddItemToObject(root, "arr_size", size_num);
    dup_arr = cJSON_Duplicate(arr, 1);
    item0 = cJSON_GetArrayItem(arr, 0);
    dup_item0 = cJSON_GetArrayItem(dup_arr, 0);
    items_equal = cJSON_Compare(item0, dup_item0, 1);
    cJSON_AddBoolToObject(root, "items_equal", items_equal);
    printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup_arr);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}