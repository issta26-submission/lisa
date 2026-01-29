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
//<ID> 470
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"label\":\"root\",\"value\":42}";
    cJSON *root = cJSON_ParseWithLength(json, sizeof(json) - 1);
    cJSON *value_item = cJSON_GetObjectItem(root, "value");
    double orig_value = cJSON_GetNumberValue(value_item);

    // step 2: Configure
    cJSON *flag = cJSON_AddTrueToObject(root, "flag");
    cJSON *refs_array = cJSON_AddArrayToObject(root, "refs");
    cJSON *pi_item = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(root, "pi", pi_item);
    cJSON_bool is_obj = cJSON_IsObject(root);
    cJSON_AddItemToObject(root, "is_object_flag", cJSON_CreateBool(is_obj));

    // step 3: Operate
    cJSON_AddItemReferenceToArray(refs_array, pi_item);
    double new_pi = cJSON_SetNumberHelper(pi_item, 2.71828);
    cJSON *sum_item = cJSON_CreateNumber(orig_value + new_pi);
    cJSON_AddItemToObject(root, "sum", sum_item);

    // step 4: Validate & Cleanup
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}