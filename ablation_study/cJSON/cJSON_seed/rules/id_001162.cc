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
//<ID> 1162
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *num_item = NULL;
    cJSON *num_item2 = NULL;
    cJSON *true_item = NULL;
    cJSON *false_item = NULL;
    double extracted_main = 0.0;
    double extracted_second = 0.0;
    int array_count = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(42.5);
    cJSON_AddItemToObject(root, "answer", num_item);
    arr = cJSON_CreateArray();
    true_item = cJSON_CreateTrue();
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, true_item);
    num_item2 = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(arr, num_item2);
    cJSON_AddItemToArray(arr, false_item);
    cJSON_AddItemToObject(root, "values", arr);

    // step 3: Operate / Validate
    extracted_main = cJSON_GetNumberValue(num_item);
    extracted_second = cJSON_GetNumberValue(num_item2);
    array_count = cJSON_GetArraySize(arr);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}