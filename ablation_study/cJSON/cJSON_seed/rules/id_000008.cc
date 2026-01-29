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
//<ID> 8
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations and initialization
    cJSON *root = cJSON_CreateObject();
    cJSON *null_item = cJSON_AddNullToObject(root, "empty");
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "is_enabled", true_item);
    cJSON *raw_item = cJSON_CreateRaw("raw_payload");
    cJSON_AddItemToObject(root, "raw_data", raw_item);

    // step 2: Operate
    cJSON_bool is_true = cJSON_IsTrue(true_item);
    cJSON_bool is_raw = cJSON_IsRaw(raw_item);
    char *json = cJSON_PrintUnformatted(root);

    // step 3: Validate
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON_bool identical = cJSON_Compare(root, dup, 1);
    int validation = (int)is_true + ((int)is_raw << 1) + ((int)identical << 2);
    (void)validation;

    // step 4: Cleanup
    cJSON_free(json);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}