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
//<ID> 333
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "numbers", arr);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "total", 3.0);
    cJSON *n3 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(arr, n3);

    // step 3: Operate & Validate
    char *printed_before = cJSON_Print(root);
    cJSON *replacement = cJSON_CreateNumber(42.0);
    cJSON_ReplaceItemInArray(arr, 1, replacement);
    cJSON *fetched_arr = cJSON_GetObjectItem(root, "numbers");
    cJSON *first_elem = cJSON_GetArrayItem(fetched_arr, 0);
    double first_val = cJSON_GetNumberValue(first_elem);
    cJSON *augmented = cJSON_CreateNumber(first_val + 10.0);
    cJSON_AddItemToObject(root, "augmented_first", augmented);
    char *printed_after = cJSON_Print(root);
    cJSON_free(printed_before);
    cJSON_free(printed_after);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}