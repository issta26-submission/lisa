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
//<ID> 805
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *raw_text = "{\"meta\": {\"version\": 1}}";
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *raw = NULL;
    cJSON *num0 = NULL;
    cJSON *num1 = NULL;
    cJSON *arr_copy = NULL;
    cJSON *item0 = NULL;
    cJSON *item1 = NULL;
    cJSON *equal_flag = NULL;
    char *printed = NULL;
    int arr_size = 0;
    cJSON_bool equal_result = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(arr, num0);
    num1 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToObject(root, "values", arr);
    raw = cJSON_CreateRaw(raw_text);
    cJSON_AddItemToObject(root, "raw_meta", raw);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    item0 = cJSON_GetArrayItem(arr, 0);
    item1 = cJSON_GetArrayItem(arr, 1);
    arr_copy = cJSON_Duplicate(arr, 1);
    equal_result = cJSON_Compare(arr, arr_copy, 1);
    equal_flag = cJSON_CreateBool(equal_result);
    cJSON_AddItemToObject(root, "copied_equal", equal_flag);
    printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(arr_copy);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}