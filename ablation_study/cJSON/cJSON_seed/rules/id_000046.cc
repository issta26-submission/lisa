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
//<ID> 46
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(10.0);
    cJSON *num2 = cJSON_CreateNumber(20.5);
    cJSON *dup_arr = NULL;
    cJSON *first_elem = NULL;
    char *printed = NULL;
    int arr_size = 0;
    double first_val = 0.0;
    cJSON_bool equal = 0;

    // step 2: Setup
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "numbers", arr);
    arr_size = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "count", (double)arr_size);

    // step 3: Core operations / Validate
    first_elem = cJSON_GetArrayItem(arr, 0);
    first_val = cJSON_GetNumberValue(first_elem);
    cJSON_AddNumberToObject(root, "first_times_two", first_val * 2.0);
    dup_arr = cJSON_Duplicate(arr, 1);
    equal = cJSON_Compare(arr, dup_arr, 1);
    cJSON_AddBoolToObject(root, "arrays_equal", equal);
    printed = cJSON_PrintBuffered(root, 256, 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup_arr);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}