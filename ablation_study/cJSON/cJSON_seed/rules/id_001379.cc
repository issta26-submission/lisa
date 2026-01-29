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
//<ID> 1379
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
    cJSON *num_item = NULL;
    cJSON *true_in_arr = NULL;
    cJSON *true_ref = NULL;
    cJSON *retrieved_item = NULL;
    int arr_size = 0;
    cJSON_bool cmp_result = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "count", 3.0);
    arr = cJSON_CreateArray();
    num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, num_item);
    true_in_arr = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, true_in_arr);
    cJSON_AddItemToObject(root, "data", arr);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    retrieved_item = cJSON_GetArrayItem(arr, 1);
    true_ref = cJSON_CreateTrue();
    cmp_result = cJSON_Compare(retrieved_item, true_ref, 1);
    (void)arr_size;
    (void)cmp_result;

    // step 4: Cleanup
    cJSON_Delete(true_ref);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}