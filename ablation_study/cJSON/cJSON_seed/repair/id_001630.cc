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
//<ID> 1630
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"config\":{\"enabled\":false,\"values\":[1,2,3]}}";
    cJSON *root = cJSON_Parse(json);
    cJSON *config = cJSON_GetObjectItemCaseSensitive(root, "config");
    cJSON *values = cJSON_GetObjectItemCaseSensitive(config, "values");
    cJSON *enabled = cJSON_GetObjectItemCaseSensitive(config, "enabled");

    // step 2: Configure
    cJSON *flag_true = cJSON_CreateBool(1);
    cJSON_AddItemToArray(values, flag_true);
    cJSON *flag_false = cJSON_CreateBool(0);
    cJSON_AddItemToArray(values, flag_false);

    // step 3: Operate and Validate
    int size_after = cJSON_GetArraySize(values);
    cJSON *size_flag = cJSON_CreateBool(size_after);
    cJSON_AddItemToArray(values, size_flag);

    // step 4: Cleanup
    (void)enabled;
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}