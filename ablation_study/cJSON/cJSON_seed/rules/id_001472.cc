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
//<ID> 1472
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
    cJSON *n0 = NULL;
    cJSON *n1 = NULL;
    cJSON *n2 = NULL;
    cJSON *retrieved_numbers = NULL;
    cJSON *array_item1 = NULL;
    cJSON *dup_item = NULL;
    cJSON *detached_item = NULL;
    cJSON *detached_ref = NULL;
    double dup_val = 0.0;
    double detached_val = 0.0;
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(arr, n0);
    n1 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(arr, n1);
    n2 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddNumberToObject(root, "count", 3.0);

    // step 3: Operate / Validate
    retrieved_numbers = cJSON_GetObjectItem(root, "numbers");
    array_item1 = cJSON_GetArrayItem(retrieved_numbers, 1);
    dup_item = cJSON_Duplicate(array_item1, 0);
    detached_item = cJSON_DetachItemViaPointer(retrieved_numbers, array_item1);
    cJSON_AddItemToObject(root, "extracted", detached_item);
    dup_val = cJSON_GetNumberValue(dup_item);
    detached_ref = cJSON_GetObjectItem(root, "extracted");
    detached_val = cJSON_GetNumberValue(detached_ref);
    (void)buffer;
    (void)dup_val;
    (void)detached_val;

    // step 4: Cleanup
    cJSON_Delete(dup_item);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}