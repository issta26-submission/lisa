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
//<ID> 139
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
    cJSON *n0 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(items, n0);
    cJSON *n1 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(items, n1);
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *name_item = cJSON_CreateString("initial");
    cJSON_AddItemToObject(meta, "name", name_item);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "scale", 2.0);
    char *updated_name = cJSON_SetValuestring(name_item, "updated");

    // step 3: Operate and Validate
    cJSON *n_insert = cJSON_CreateNumber(5.0);
    cJSON_InsertItemInArray(items, 1, n_insert);
    cJSON *it0 = cJSON_GetArrayItem(items, 0);
    cJSON *it1 = cJSON_GetArrayItem(items, 1);
    cJSON *it2 = cJSON_GetArrayItem(items, 2);
    double v0 = cJSON_GetNumberValue(it0);
    double v1 = cJSON_GetNumberValue(it1);
    double v2 = cJSON_GetNumberValue(it2);
    double sum = v0 + v1 + v2;
    cJSON *scale_obj = cJSON_GetObjectItem(root, "scale");
    double scale_val = cJSON_GetNumberValue(scale_obj);
    double scaled_sum = sum * scale_val;
    cJSON_AddNumberToObject(root, "scaled_sum", scaled_sum);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    (void)updated_name;
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}