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
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);
    cJSON *item0 = cJSON_CreateNumber(1.5);
    cJSON_InsertItemInArray(values, 0, item0);
    cJSON *item1 = cJSON_CreateNumber(2.5);
    cJSON_InsertItemInArray(values, 1, item1);
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *label = cJSON_CreateString("initial");
    cJSON_AddItemToObject(meta, "label", label);

    // step 2: Configure
    char *updated = cJSON_SetValuestring(label, "updated");
    (void)updated;
    cJSON_AddNumberToObject(meta, "count", 2.0);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(values, 0);
    cJSON *second = cJSON_GetArrayItem(values, 1);
    double v0 = cJSON_GetNumberValue(first);
    double v1 = cJSON_GetNumberValue(second);
    double total = v0 + v1;
    cJSON_AddNumberToObject(root, "sum", total);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}