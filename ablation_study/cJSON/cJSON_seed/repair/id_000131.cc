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
//<ID> 131
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON_AddNumberToObject(config, "interval", 10.0);
    cJSON *state = cJSON_CreateString("init");
    cJSON_AddItemToObject(config, "state", state);
    cJSON_SetValuestring(state, "running");
    cJSON *n1 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(items, n1);
    cJSON *n2 = cJSON_CreateNumber(7.5);
    cJSON_InsertItemInArray(items, 1, n2);
    cJSON *n3 = cJSON_CreateNumber(5.0);
    cJSON_AddItemToArray(items, n3);

    // step 3: Operate and Validate
    cJSON *a0 = cJSON_GetArrayItem(items, 0);
    cJSON *a1 = cJSON_GetArrayItem(items, 1);
    double v0 = cJSON_GetNumberValue(a0);
    double v1 = cJSON_GetNumberValue(a1);
    double total = v0 + v1;
    cJSON_AddNumberToObject(root, "sum_first_two", total);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}