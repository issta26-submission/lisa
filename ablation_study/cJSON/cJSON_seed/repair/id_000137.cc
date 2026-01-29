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
//<ID> 137
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *cfg = cJSON_AddObjectToObject(root, "config");
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);
    cJSON *name = cJSON_CreateString("initial");
    cJSON_AddItemToObject(cfg, "name", name);
    cJSON_AddNumberToObject(cfg, "version", 1.0);

    // step 2: Configure
    cJSON *num0 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(values, num0);
    cJSON *num1 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(values, num1);
    cJSON *num_mid = cJSON_CreateNumber(15.0);
    cJSON_InsertItemInArray(values, 1, num_mid);

    // step 3: Operate and Validate
    cJSON *name_item = cJSON_GetObjectItem(cfg, "name");
    cJSON_SetValuestring(name_item, "updated");
    cJSON *a0 = cJSON_GetArrayItem(values, 0);
    cJSON *a1 = cJSON_GetArrayItem(values, 1);
    cJSON *a2 = cJSON_GetArrayItem(values, 2);
    double v0 = cJSON_GetNumberValue(a0);
    double v1 = cJSON_GetNumberValue(a1);
    double v2 = cJSON_GetNumberValue(a2);
    double total = v0 + v1 + v2;
    cJSON_AddNumberToObject(root, "sum", total);
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *note = cJSON_CreateString("values_updated");
    cJSON_AddItemToObject(meta, "note", note);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}