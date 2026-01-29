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
//<ID> 130
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *items = (cJSON *)0;
    cJSON *n1 = (cJSON *)0;
    cJSON *n2 = (cJSON *)0;
    cJSON *n3 = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *tmp_number = (cJSON *)0;
    cJSON_bool is_third_null = (cJSON_bool)0;
    int size_before = 0;
    int size_after = 0;
    char *json_text = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);

    // step 3: Configure
    n1 = cJSON_CreateNumber(10.5);
    cJSON_AddItemToArray(items, n1);
    n2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(items, n2);
    n3 = cJSON_CreateNull();
    cJSON_AddItemToArray(items, n3);
    size_before = cJSON_GetArraySize(items);
    tmp_number = cJSON_AddNumberToObject(root, "count", (double)size_before);

    // step 4: Operate
    is_third_null = cJSON_IsNull(n3);
    detached = cJSON_DetachItemFromArray(items, 1);
    size_after = cJSON_GetArraySize(items);
    tmp_number = cJSON_AddNumberToObject(root, "count_after_detach", (double)size_after);
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 5: Validate
    cJSON_AddNumberToObject(root, "third_was_null", (double)is_third_null);
    cJSON_AddNumberToObject(root, "detached_was_null", (double)cJSON_IsNull(detached));

    // step 6: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}