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
//<ID> 136
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *values = (cJSON *)0;
    cJSON *n1 = (cJSON *)0;
    cJSON *n2 = (cJSON *)0;
    cJSON *moved_array = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *count_node = (cJSON *)0;
    cJSON_bool detached_is_null = (cJSON_bool)0;
    int size_before = 0;
    int size_after = 0;
    char *json_text = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);

    // step 3: Configure - create and populate number items, add a count number to object
    n1 = cJSON_CreateNumber(10.0);
    n2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    count_node = cJSON_AddNumberToObject(root, "count_estimated", 2.0);

    // step 4: Operate - inspect array size, detach first item and move it to a new array
    size_before = cJSON_GetArraySize(values);
    detached = cJSON_DetachItemFromArray(values, 0);
    moved_array = cJSON_CreateArray();
    cJSON_AddItemToArray(moved_array, detached);
    cJSON_AddItemToObject(root, "moved", moved_array);

    // step 5: Validate - check sizes and nullness, produce JSON text and free it
    size_after = cJSON_GetArraySize(values);
    detached_is_null = cJSON_IsNull(detached);
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}