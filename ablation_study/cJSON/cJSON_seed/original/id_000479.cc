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
//<ID> 479
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"items\":[10,20],\"info\":{}}";
    cJSON *root = cJSON_ParseWithLength(json, sizeof(json) - 1);
    cJSON *items = cJSON_GetObjectItem(root, "items");
    cJSON *info = cJSON_GetObjectItem(root, "info");
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    double orig_first = cJSON_GetNumberValue(first_item);

    // step 2: Configure
    cJSON *extras = cJSON_AddArrayToObject(root, "extras");
    cJSON *new_num = cJSON_CreateNumber(0.0);
    cJSON_bool info_is_object = cJSON_IsObject(info);
    double set_val = 99.0 + (double)info_is_object;
    cJSON_SetNumberHelper(new_num, set_val);
    cJSON_AddItemToObject(root, "newnum", new_num);
    cJSON_AddTrueToObject(info, "valid");

    // step 3: Operate
    cJSON_AddItemReferenceToArray(extras, first_item);
    cJSON_AddItemReferenceToArray(extras, new_num);
    char *snapshot = cJSON_PrintUnformatted(root);
    double extra0 = cJSON_GetNumberValue(cJSON_GetArrayItem(extras, 0));
    double extra1 = cJSON_GetNumberValue(cJSON_GetArrayItem(extras, 1));
    double sum = orig_first + extra0 + extra1;

    // step 4: Validate & Cleanup
    (void)sum;
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}