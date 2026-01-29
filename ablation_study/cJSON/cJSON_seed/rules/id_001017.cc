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
//<ID> 1017
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *values = NULL;
    cJSON *meta_old = NULL;
    cJSON *meta_new = NULL;
    cJSON *valid_item = NULL;
    cJSON *got_valid = NULL;
    cJSON *first_val_item = NULL;
    char *printed = NULL;
    const double nums[3] = { 1.1, 2.2, 3.3 };
    const char *raw_json = "{\"x\":10}";
    double first_val = 0.0;
    cJSON_bool valid_flag = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    values = cJSON_CreateDoubleArray(nums, 3);
    cJSON_AddItemToObject(root, "values", values);
    valid_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "valid", valid_item);
    meta_old = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta_old);
    cJSON_AddStringToObject(meta_old, "name", "sensor-1");
    meta_new = cJSON_CreateRaw(raw_json);
    cJSON_ReplaceItemViaPointer(root, meta_old, meta_new);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    got_valid = cJSON_GetObjectItem(root, "valid");
    valid_flag = cJSON_IsTrue(got_valid);
    first_val_item = cJSON_GetArrayItem(cJSON_GetObjectItem(root, "values"), 0);
    first_val = cJSON_GetNumberValue(first_val_item);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}