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
//<ID> 630
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
    cJSON *raw_added = cJSON_AddRawToObject(root, "raw", "{\"raw_key\":123}");
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(settings, "answer", num);

    // step 2: Configure
    cJSON *settings_dup = cJSON_Duplicate(settings, 1);
    cJSON_AddItemToObject(root, "settings_copy", settings_dup);

    // step 3: Operate
    cJSON_bool is_orig_object = cJSON_IsObject(settings);
    cJSON_bool is_dup_object = cJSON_IsObject(settings_dup);
    cJSON_bool compare_equal = cJSON_Compare((const cJSON *)settings, (const cJSON *)settings_dup, 1);
    char *unformatted = cJSON_PrintUnformatted(root);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);

    // step 4: Validate & Cleanup
    (void)is_orig_object;
    (void)is_dup_object;
    (void)compare_equal;
    (void)raw_added;
    cJSON_free(unformatted);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}