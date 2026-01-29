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
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *label = cJSON_CreateString("placeholder");

    // step 2: Configure
    char *set_res = cJSON_SetValuestring(label, "updated");
    cJSON_AddItemToObject(config, "label", label);
    cJSON_AddNumberToObject(config, "threshold", 3.14);

    // step 3: Operate and Validate
    cJSON *n0 = cJSON_CreateNumber(2.5);
    cJSON_InsertItemInArray(items, 0, n0);
    cJSON *n1 = cJSON_CreateNumber(4.75);
    cJSON_InsertItemInArray(items, 1, n1);
    cJSON *first = cJSON_GetArrayItem(items, 0);
    double v0 = cJSON_GetNumberValue(first);
    cJSON *second = cJSON_GetArrayItem(items, 1);
    double v1 = cJSON_GetNumberValue(second);
    double total = v0 + v1;
    cJSON_AddNumberToObject(root, "sum", total);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}