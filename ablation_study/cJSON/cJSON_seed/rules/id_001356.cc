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
//<ID> 1356
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *config = NULL;
    cJSON *enabled = NULL;
    cJSON *threshold_item = NULL;
    const char *version = NULL;
    const char *ver_str = NULL;
    double threshold_val = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    config = cJSON_AddObjectToObject(root, "config");
    cJSON_AddNumberToObject(config, "threshold", 7.5);
    enabled = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", enabled);
    version = cJSON_Version();
    cJSON_AddStringToObject(root, "lib_version", version);

    // step 3: Operate / Validate
    threshold_item = cJSON_GetObjectItem(config, "threshold");
    threshold_val = cJSON_GetNumberValue(threshold_item);
    ver_str = cJSON_GetStringValue(cJSON_GetObjectItem(root, "lib_version"));
    (void)threshold_val;
    (void)ver_str;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}