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
//<ID> 473
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"value\":10,\"meta\":{\"valid\":false}}";
    cJSON *root = cJSON_ParseWithLength(json, sizeof(json) - 1);
    cJSON *value_item = cJSON_GetObjectItem(root, "value");
    double orig_value = cJSON_GetNumberValue(value_item);
    cJSON *meta = cJSON_GetObjectItem(root, "meta");
    cJSON_bool meta_is_obj = cJSON_IsObject(meta);

    // step 2: Configure
    cJSON_SetNumberHelper(value_item, 88.0);
    cJSON *refs = cJSON_AddArrayToObject(root, "refs");
    cJSON_AddItemReferenceToArray(refs, value_item);
    cJSON *flag = cJSON_AddTrueToObject(root, "enabled");
    cJSON *newnum = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(newnum, 123.456);
    cJSON_AddItemToArray(refs, newnum);

    // step 3: Operate
    double ref0 = cJSON_GetNumberValue(cJSON_GetArrayItem(refs, 0));
    double ref1 = cJSON_GetNumberValue(cJSON_GetArrayItem(refs, 1));
    double sum = orig_value + ref0 + ref1;
    cJSON *sum_item = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(sum_item, sum);
    cJSON_AddItemToObject(root, "sum", sum_item);
    cJSON_bool root_is_obj = cJSON_IsObject(root);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    (void)meta_is_obj;
    (void)flag;
    (void)root_is_obj;
    (void)ref0;
    (void)ref1;
    (void)sum;
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}