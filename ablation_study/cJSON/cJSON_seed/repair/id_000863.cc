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
//<ID> 863
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
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON_AddStringToObject(root, "id", "node42");
    cJSON_AddNumberToObject(root, "version", 1.0);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(settings, "maybe", maybe_null);

    // step 2: Configure
    cJSON *flag_true = cJSON_CreateTrue();
    cJSON_AddItemToObject(settings, "flag", flag_true);
    cJSON *threshold = cJSON_CreateNumber(3.14);
    cJSON_AddItemToObject(settings, "threshold", threshold);
    cJSON_AddStringToObject(settings, "note", "configuration_sample");

    // step 3: Operate and Validate
    cJSON *ver_item = cJSON_GetObjectItem(root, "version");
    double ver_val = cJSON_GetNumberValue(ver_item);
    cJSON_AddNumberToObject(root, "version_copy", ver_val);
    cJSON *flag_item = cJSON_GetObjectItem(settings, "flag");
    cJSON_bool flag_truth = cJSON_IsTrue(flag_item);
    double flag_val = (double)flag_truth;
    cJSON_AddNumberToObject(root, "flag_truth", flag_val);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}