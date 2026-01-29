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
//<ID> 1611
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = NULL;
    cJSON *num_item = NULL;
    cJSON *raw_item = NULL;
    cJSON *null_item = NULL;
    char *out = NULL;
    double extracted = 0.0;
    cJSON_bool added_raw = 0;
    cJSON_bool added_maybe = 0;
    cJSON_bool is_null = 0;
    cJSON_bool validation_flag = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num_item = cJSON_AddNumberToObject(root, "pi", 3.141592653589793);
    raw_item = cJSON_CreateRaw("{\"embedded\":true}");
    added_raw = cJSON_AddItemToObject(root, "payload", raw_item);
    null_item = cJSON_CreateNull();
    added_maybe = cJSON_AddItemToObject(root, "maybe", null_item);

    // step 3: Operate / Validate
    out = cJSON_PrintUnformatted(root);
    extracted = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "pi"));
    is_null = cJSON_IsNull(cJSON_GetObjectItem(root, "maybe"));
    validation_flag = (added_raw && added_maybe && (extracted > 3.14) && is_null);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)num_item;
    (void)raw_item;
    (void)null_item;
    (void)validation_flag;
    return 66;
}