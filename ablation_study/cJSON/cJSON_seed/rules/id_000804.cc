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
//<ID> 804
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
    cJSON *num = NULL;
    cJSON *raw = NULL;
    cJSON *array_copy = NULL;
    cJSON *first_item = NULL;
    char *printed = NULL;
    int arr_size = 0;
    cJSON_bool equal = 0;
    double first_value = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num = cJSON_CreateNumber(1.0);
    cJSON_AddItemToArray(arr, num);
    num = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, num);
    num = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(arr, num);
    cJSON_AddItemToObject(root, "numbers", arr);
    raw = cJSON_CreateRaw(raw_json);
    cJSON_AddItemToObject(root, "meta_raw", raw);
    array_copy = cJSON_Duplicate(arr, 1);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "arr_size", (double)arr_size);
    first_item = cJSON_GetArrayItem(arr, 1);
    first_value = cJSON_GetNumberValue(first_item);
    equal = cJSON_Compare(arr, array_copy, 1);
    cJSON_AddItemToObject(root, "arrays_equal", cJSON_CreateBool(equal));
    printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(array_copy);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}