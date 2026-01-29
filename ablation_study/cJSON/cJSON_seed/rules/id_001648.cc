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
//<ID> 1648
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
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *arr_ref = NULL;
    cJSON *found_values = NULL;
    int array_size = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", arr);
    num1 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(arr, num1);
    num2 = cJSON_CreateNumber(2.718);
    cJSON_AddItemToArray(arr, num2);
    arr_ref = cJSON_CreateArrayReference(arr);
    cJSON_AddItemToObject(root, "values_ref", arr_ref);

    // step 3: Operate / Validate
    found_values = cJSON_GetObjectItem(root, "values");
    array_size = cJSON_GetArraySize(found_values);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}