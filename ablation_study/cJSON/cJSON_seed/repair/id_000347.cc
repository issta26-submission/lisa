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
//<ID> 347
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
    cJSON *flag = cJSON_CreateTrue();
    cJSON *name = cJSON_CreateString("mode");
    cJSON_AddItemToObject(config, "flag", flag);
    cJSON_AddItemToObject(config, "name", name);
    cJSON_AddItemToObject(root, "config", config);

    // step 2: Configure
    cJSON_bool is_flag_true = cJSON_IsTrue(flag);
    const char *name_key = cJSON_GetStringValue(name);
    cJSON *flag_score = cJSON_CreateNumber(is_flag_true ? 100.0 : 0.0);
    cJSON_AddItemToObject(root, name_key, flag_score);

    // step 3: Operate and Validate
    cJSON *pi = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(root, "pi", pi);
    double score_val = cJSON_GetNumberValue(flag_score);
    cJSON *doubled = cJSON_CreateNumber(score_val * 2.0);
    cJSON_AddItemToObject(root, "doubled_score", doubled);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}