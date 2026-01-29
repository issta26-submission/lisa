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
//<ID> 1357
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_AddObjectToObject(root, "settings");
    cJSON *note_null = cJSON_CreateNull();
    float thresholds_vals[3] = {1.5f, 2.5f, 3.25f};
    cJSON *thresholds = cJSON_CreateFloatArray(thresholds_vals, 3);

    // step 2: Configure
    cJSON_AddItemToObject(settings, "note", note_null);
    cJSON_AddItemToObject(settings, "thresholds", thresholds);
    cJSON_AddTrueToObject(settings, "enabled");
    cJSON *metadata = cJSON_AddObjectToObject(root, "metadata");
    cJSON_AddNumberToObject(metadata, "version", 1.0);

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);

    // step 4: Validate & Cleanup
    cJSON_bool enabled_flag = cJSON_IsTrue(cJSON_GetObjectItem(settings, "enabled"));
    cJSON *marker = cJSON_CreateNumber((double)enabled_flag);
    cJSON_AddItemToObject(root, "enabled_marker", marker);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}