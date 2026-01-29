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
//<ID> 132
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
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *num3 = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    char *json_text = (char *)0;
    int size_before = 0;
    int size_after = 0;
    cJSON_bool detached_is_null = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    array = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(10.0);
    num2 = cJSON_CreateNumber(20.0);
    num3 = cJSON_CreateNumber(30.0);

    // step 3: Configure - build array and attach to root
    cJSON_AddItemToArray(array, num1);
    cJSON_AddItemToArray(array, num2);
    cJSON_AddItemToArray(array, num3);
    cJSON_AddItemToObject(root, "values", array);

    // step 4: Operate - measure, detach an item, and record sizes
    size_before = cJSON_GetArraySize(array);
    detached = cJSON_DetachItemFromArray(array, 1);
    detached_is_null = cJSON_IsNull(detached);
    size_after = cJSON_GetArraySize(array);
    cJSON_AddNumberToObject(root, "size_before", (double)size_before);
    cJSON_AddNumberToObject(root, "size_after", (double)size_after);
    cJSON_AddNumberToObject(root, "detached_was_null", (double)detached_is_null);

    // step 5: Operate - produce output and release the print buffer
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}