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
//<ID> 847
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *numbers = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", numbers);

    // step 2: Configure
    cJSON *num_item = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(num_item, 42.0);
    cJSON_AddItemToArray(numbers, num_item);
    cJSON *obj_item = cJSON_CreateObject();
    cJSON_AddStringToObject(obj_item, "name", "sample");
    cJSON_AddItemToArray(numbers, obj_item);
    cJSON_AddItemToObject(root, "placeholder", cJSON_CreateString("old_value"));

    // step 3: Operate
    cJSON *replacement = cJSON_CreateNumber(100.5);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "placeholder", replacement);
    char mutable_json[] = " { \"x\" :  1 , \"y\" :2 } ";
    cJSON_Minify(mutable_json);
    char *out = cJSON_PrintUnformatted(root);
    double replaced_val = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(root, "placeholder"));
    (void)replaced_val;
    cJSON_free(out);

    // step 4: Validate & Cleanup
    int count = cJSON_GetArraySize(numbers);
    (void)count;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}