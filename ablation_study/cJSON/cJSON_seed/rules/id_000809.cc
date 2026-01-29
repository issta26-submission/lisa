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
//<ID> 809
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
    cJSON *n1 = NULL;
    cJSON *n2 = NULL;
    cJSON *n3 = NULL;
    cJSON *raw = NULL;
    cJSON *dup_arr = NULL;
    cJSON *picked_item = NULL;
    char *printed = NULL;
    int arr_size = 0;
    double picked_value = 0.0;
    cJSON_bool arrays_equal = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(arr, n1);
    n2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(arr, n2);
    n3 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(arr, n3);
    cJSON_AddItemToObject(root, "numbers", arr);
    raw = cJSON_CreateRaw("{\"meta\":true}");
    cJSON_AddItemToObject(root, "meta_raw", raw);

    // step 3: Operate / Validate
    printed = cJSON_Print(root);
    arr_size = cJSON_GetArraySize(arr);
    dup_arr = cJSON_Duplicate(arr, 1);
    picked_item = cJSON_GetArrayItem(dup_arr, 1);
    picked_value = cJSON_GetNumberValue(picked_item);
    cJSON_AddNumberToObject(root, "picked_value", picked_value);
    arrays_equal = cJSON_Compare(arr, dup_arr, 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup_arr);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)arr_size;
    (void)arrays_equal;
    return 66;
}