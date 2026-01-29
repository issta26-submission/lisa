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
//<ID> 636
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = "{\"inner\": {\"x\": 10}, \"list\": [1,2,3]}";
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON *raw_item = cJSON_AddRawToObject(config, "data", raw_json);

    // step 2: Configure
    cJSON *other = cJSON_CreateObject();
    cJSON_AddRawToObject(other, "data", raw_json);
    cJSON_bool root_is_object = cJSON_IsObject(root);

    // step 3: Operate
    cJSON *dup_config = cJSON_Duplicate(config, 1);
    cJSON_bool config_eq_other = cJSON_Compare(config, other, 1);
    cJSON_bool dup_eq_other = cJSON_Compare(dup_config, other, 1);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    (void)root_is_object;
    (void)raw_item;
    (void)config_eq_other;
    (void)dup_eq_other;
    cJSON_free(out);
    cJSON_Delete(dup_config);
    cJSON_Delete(other);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}