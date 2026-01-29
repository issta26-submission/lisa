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
//<ID> 943
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
    cJSON *n1 = cJSON_CreateNumber(42.0);
    cJSON *n2 = cJSON_CreateNumber(3.14);
    cJSON_InsertItemInArray(arr, 0, n1);
    cJSON_InsertItemInArray(arr, 1, n2);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddNumberToObject(root, "initial_count", 2.0);

    // step 2: Configure
    cJSON *n3 = cJSON_CreateNumber(7.5);
    cJSON_InsertItemInArray(arr, 2, n3);
    cJSON_AddNumberToObject(root, "after_insert_count", 3.0);

    // step 3: Operate
    cJSON *picked = cJSON_GetArrayItem(arr, 1);
    double picked_val = cJSON_GetNumberValue(picked);
    cJSON_AddNumberToObject(root, "extracted_value", picked_val);
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON_PrintPreallocated(root, buffer, 256, 1);

    // step 4: Validate & Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}