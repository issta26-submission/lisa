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
//<ID> 134
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data_arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "data", data_arr);
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddNumberToObject(meta, "version", 1.0);
    cJSON *name_item = cJSON_CreateString("initial");
    cJSON_AddItemToObject(meta, "name", name_item);

    // step 2: Configure
    cJSON_SetValuestring(name_item, "updated");
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(data_arr, n1);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON_InsertItemInArray(data_arr, 0, n2);
    cJSON *details = cJSON_AddObjectToObject(meta, "details");
    cJSON_AddNumberToObject(details, "count", 2.0);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(data_arr, 0);
    cJSON *second = cJSON_GetArrayItem(data_arr, 1);
    double v1 = cJSON_GetNumberValue(first);
    double v2 = cJSON_GetNumberValue(second);
    double sum = v1 + v2;
    cJSON_AddNumberToObject(root, "sum", sum);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}