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
//<ID> 1184
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *parsed = NULL;
    cJSON *config = NULL;
    cJSON *enabled = NULL;
    cJSON *threshold_item = NULL;
    cJSON *replacement = NULL;
    cJSON *confirmed = NULL;
    cJSON *temp = NULL;
    char *printed = NULL;
    double threshold = 0.0;
    cJSON_bool confirmed_flag = 0;
    const char json[] = "{\"config\":{\"enabled\":false,\"threshold\":10},\"flag\":false}";

    // step 2: Setup / Configure
    parsed = cJSON_Parse(json);
    config = cJSON_GetObjectItem(parsed, "config");
    enabled = cJSON_GetObjectItem(config, "enabled");
    threshold_item = cJSON_GetObjectItem(config, "threshold");
    threshold = cJSON_GetNumberValue(threshold_item);
    replacement = cJSON_CreateTrue();
    cJSON_ReplaceItemViaPointer(config, enabled, replacement);
    confirmed = cJSON_AddTrueToObject(parsed, "confirmed");
    temp = cJSON_CreateString("ephemeral");

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(parsed);
    confirmed_flag = cJSON_IsTrue(cJSON_GetObjectItem(parsed, "confirmed"));
    (void)threshold;
    (void)confirmed_flag;
    cJSON_Delete(temp);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}