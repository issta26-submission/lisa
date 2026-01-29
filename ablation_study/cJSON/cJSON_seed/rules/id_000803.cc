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
//<ID> 803
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *raw_json = "{\"meta\":true}";
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *num3 = NULL;
    cJSON *raw_item = NULL;
    cJSON *array_copy = NULL;
    cJSON *item1 = NULL;
    cJSON *size_num = NULL;
    cJSON *scaled = NULL;
    char *printed = NULL;
    int arr_size = 0;
    double item_value = 0.0;
    cJSON_bool cmp = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(1.0);
    cJSON_AddItemToArray(arr, num1);
    num2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, num2);
    num3 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(arr, num3);
    cJSON_AddItemToObject(root, "numbers", arr);
    raw_item = cJSON_CreateRaw(raw_json);
    cJSON_AddItemToObject(root, "metadata", raw_item);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    item1 = cJSON_GetArrayItem(arr, 1);
    item_value = cJSON_GetNumberValue(item1);
    array_copy = cJSON_Duplicate(arr, 1);
    cmp = cJSON_Compare(arr, array_copy, 1);
    size_num = cJSON_CreateNumber((double)arr_size);
    cJSON_AddItemToObject(root, "arr_size", size_num);
    scaled = cJSON_CreateNumber(item_value * 10.0);
    cJSON_AddItemToObject(root, "scaled_second", scaled);
    printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(array_copy);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}