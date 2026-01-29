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
//<ID> 386
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *cfg = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "value", 42.5);
    cJSON_AddBoolToObject(root, "flag", 1);
    cJSON_AddItemToObject(root, "config", cfg);
    cJSON_AddNumberToObject(cfg, "threshold", 3.14159);

    // step 2: Configure
    cJSON *got_value = cJSON_GetObjectItemCaseSensitive(root, "value");
    cJSON *got_flag = cJSON_GetObjectItemCaseSensitive(root, "flag");
    cJSON *got_config = cJSON_GetObjectItemCaseSensitive(root, "config");
    cJSON *got_threshold = cJSON_GetObjectItemCaseSensitive(got_config, "threshold");
    double v = cJSON_GetNumberValue(got_value);
    cJSON_bool flag = cJSON_IsTrue(got_flag);
    double t = cJSON_GetNumberValue(got_threshold);

    // step 3: Operate
    double combined = v + t + (double)flag;
    cJSON *summary = cJSON_CreateObject();
    cJSON_AddNumberToObject(summary, "combined", combined);
    cJSON_AddNumberToObject(summary, "value", v);
    cJSON_AddNumberToObject(summary, "threshold", t);
    cJSON_AddBoolToObject(summary, "flag", flag);
    cJSON_AddItemToObject(root, "summary", summary);

    // step 4: Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}