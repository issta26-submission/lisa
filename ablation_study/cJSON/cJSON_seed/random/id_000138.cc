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
//<ID> 138
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *array = (cJSON *)0;
    cJSON *n1 = (cJSON *)0;
    cJSON *n2 = (cJSON *)0;
    cJSON *n3 = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON_bool was_null = (cJSON_bool)0;
    int original_size = 0;
    int remaining_size = 0;
    char *printed = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", array);

    // step 3: Configure
    n1 = cJSON_CreateNumber(10.0);
    n2 = cJSON_CreateNumber(20.0);
    n3 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(array, n1);
    cJSON_AddItemToArray(array, n2);
    cJSON_AddItemToArray(array, n3);
    original_size = cJSON_GetArraySize(array);

    // step 4: Operate & Validate
    detached = cJSON_DetachItemFromArray(array, 1);
    was_null = cJSON_IsNull(detached);
    remaining_size = cJSON_GetArraySize(array);
    cJSON_AddNumberToObject(root, "original_count", (double)original_size);
    cJSON_AddNumberToObject(root, "remaining_count", (double)remaining_size);
    cJSON_AddNumberToObject(root, "detached_was_null", (double)was_null);

    // step 5: Operate
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}