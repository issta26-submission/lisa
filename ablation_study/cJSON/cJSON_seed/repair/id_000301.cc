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
//<ID> 301
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON *name_item = cJSON_CreateString("deviceA");

    // step 2: Configure
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON *version_item = cJSON_AddNumberToObject(root, "version", 3.14);
    cJSON *threshold_item = cJSON_AddNumberToObject(settings, "threshold", 7.5);

    // step 3: Operate and Validate
    cJSON *got_name = cJSON_GetObjectItem(root, "name");
    const char *name_str = cJSON_GetStringValue(got_name);
    char *unformatted = cJSON_PrintUnformatted(root);
    char *formatted = cJSON_Print(root);
    double version_val = cJSON_GetNumberValue(version_item);
    double threshold_val = cJSON_GetNumberValue(cJSON_GetObjectItem(settings, "threshold"));
    (void)name_str;
    (void)version_val;
    (void)threshold_val;
    (void)got_name;

    // step 4: Cleanup
    cJSON_free(unformatted);
    cJSON_free(formatted);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}