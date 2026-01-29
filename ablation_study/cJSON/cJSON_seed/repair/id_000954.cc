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
//<ID> 954
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
    cJSON_AddItemToObject(root, "items", items);
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(child, "flag", cJSON_CreateTrue());
    cJSON_AddItemToObject(child, "pi", cJSON_CreateNumber(3.1415));
    cJSON_AddItemToArray(items, child);
    cJSON *num_item = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(items, num_item);

    // step 2: Configure
    cJSON *ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemToObject(root, "child_ref", ref);
    cJSON_AddStringToObject(root, "name", "fuzz_target");

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(items, 0);
    char *first_snapshot = cJSON_PrintUnformatted(first);
    char *root_snapshot = cJSON_PrintUnformatted(root);
    double second_value = cJSON_GetNumberValue(cJSON_GetArrayItem(items, 1));
    (void)second_value;
    cJSON_free(first_snapshot);
    cJSON_free(root_snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}