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
//<ID> 166
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
    cJSON_AddItemToObject(root, "numbers", arr);

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON *n2 = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(n2, 2.5);
    cJSON_AddItemToArray(arr, n2);
    cJSON *obj = cJSON_CreateObject();
    cJSON_AddNumberToObject(obj, "inner", 7.0);
    cJSON_AddItemToArray(arr, obj);
    cJSON_AddItemReferenceToArray(arr, n1);

    // step 3: Operate & Validate
    int size = cJSON_GetArraySize(arr);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON_AddNumberToObject(root, "first_value", first_val);
    cJSON *last = cJSON_GetArrayItem(arr, size - 1);
    cJSON *last_dup = cJSON_Duplicate(last, 1);
    cJSON_AddItemToObject(root, "last_duplicate", last_dup);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}