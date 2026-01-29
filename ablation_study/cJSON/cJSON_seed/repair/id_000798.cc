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
//<ID> 798
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(items, num_item);
    cJSON *str_item = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(items, str_item);
    cJSON *obj_item = cJSON_CreateObject();
    cJSON_AddItemToArray(items, obj_item);

    // step 2: Configure
    cJSON_AddStringToObject(meta, "version", "1.0");
    cJSON_AddNumberToObject(meta, "count", 3.0);
    cJSON *subflag = cJSON_AddTrueToObject(obj_item, "ok");

    // step 3: Operate and Validate
    cJSON *third = cJSON_GetArrayItem(items, 2);
    cJSON_bool third_invalid = cJSON_IsInvalid(third);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);
    (void)third_invalid;
    (void)subflag;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}