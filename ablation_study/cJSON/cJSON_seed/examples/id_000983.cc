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
//<ID> 983
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON *mode = cJSON_CreateString("auto");
    cJSON *active = cJSON_CreateFalse();
    cJSON *threshold = cJSON_CreateNumber(10.0);

    // step 2: Configure
    cJSON_AddItemToObject(root, "config", config);
    cJSON_AddItemToObject(config, "mode", mode);
    cJSON_AddItemToObject(config, "threshold", threshold);
    cJSON_AddItemToObject(root, "active", active);

    // step 3: Operate & Validate
    cJSON *got_active = cJSON_GetObjectItem(root, "active");
    cJSON_bool was_false = cJSON_IsFalse(got_active);
    double validated_score = 1.0 - (double)was_false;
    cJSON *validated_num = cJSON_CreateNumber(validated_score);
    cJSON *validated_flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "validated", validated_flag);
    cJSON_AddItemToObject(config, "validated_score", validated_num);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}