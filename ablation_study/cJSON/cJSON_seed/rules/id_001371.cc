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
//<ID> 1371
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
    cJSON *true_item = NULL;
    cJSON *num_item = NULL;
    cJSON *retrieved_true = NULL;
    cJSON *arr_item0 = NULL;
    char *printed = NULL;
    int arr_size = 0;
    cJSON_bool are_equal = 0;
    double second_value = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "count", 7.0);
    arr = cJSON_CreateArray();
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, true_item);
    num_item = cJSON_CreateNumber(123.456);
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    arr_item0 = cJSON_GetArrayItem(arr, 0);
    retrieved_true = cJSON_CreateTrue();
    are_equal = cJSON_Compare(arr_item0, retrieved_true, 1);
    second_value = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1));
    printed = cJSON_PrintUnformatted(root);
    (void)arr_size;
    (void)second_value;
    (void)printed;
    (void)are_equal;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(retrieved_true);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}