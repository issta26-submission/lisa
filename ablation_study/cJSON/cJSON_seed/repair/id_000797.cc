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
//<ID> 797
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON_AddNumberToObject(meta, "version", 1.0);
    cJSON_AddStringToObject(meta, "name", "fuzz_target");
    cJSON_AddTrueToObject(meta, "enabled");
    cJSON *num1 = cJSON_CreateNumber(10.0);
    cJSON *num2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(items, num1);
    cJSON_AddItemToArray(items, num2);

    // step 3: Operate and Validate
    cJSON *second_item = cJSON_GetArrayItem(items, 1);
    cJSON_bool second_invalid = cJSON_IsInvalid(second_item);
    (void)second_invalid;
    cJSON *snapshot = cJSON_Duplicate(root, 1);
    char *out = cJSON_PrintUnformatted(snapshot);
    cJSON_free(out);
    cJSON_Delete(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}