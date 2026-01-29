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
//<ID> 1250
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *numbers = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(42.0);
    cJSON *n2 = cJSON_CreateNumber(7.0);
    cJSON *n3 = cJSON_CreateNumber(100.0);
    cJSON_AddItemToArray(numbers, n1);
    cJSON_AddItemToArray(numbers, n2);
    cJSON_AddItemToArray(numbers, n3);
    cJSON_AddItemToObject(root, "numbers", numbers);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *name_ref = cJSON_CreateStringReference("example");
    cJSON_AddItemToObject(root, "name", name_ref);

    // step 2: Configure
    cJSON_AddStringToObject(root, "greeting", "hello");
    cJSON_AddStringToObject(meta, "info", "meta_info");
    cJSON_AddBoolToObject(root, "active", 1);

    // step 3: Operate & Validate
    cJSON *second_item = cJSON_GetArrayItem(numbers, 1);
    double second_val = cJSON_GetNumberValue(second_item);
    double computed = second_val * 2.0;
    cJSON_AddNumberToObject(root, "computed", computed);
    cJSON *detached_name = cJSON_DetachItemFromObjectCaseSensitive(root, "name");
    cJSON_AddItemToObject(meta, "detached_name", detached_name);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}