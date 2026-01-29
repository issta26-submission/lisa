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
//<ID> 1214
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"arr\":[1,2,3],\"val\":42}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *parsed_arr = cJSON_GetObjectItem(parsed, "arr");
    cJSON *parsed_val_item = cJSON_GetObjectItem(parsed, "val");
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *arr_ref = cJSON_CreateArrayReference(parsed_arr);
    cJSON_AddItemToObject(root, "arr_ref", arr_ref);
    double val = cJSON_GetNumberValue(parsed_val_item);
    cJSON_AddNumberToObject(root, "migrated_val", val);
    cJSON_DeleteItemFromObject(parsed, "val");

    // step 3: Operate & Validate
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}