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
//<ID> 207
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

    // step 2: Configure
    cJSON *features = cJSON_AddArrayToObject(settings, "features");
    cJSON *f_false = cJSON_CreateFalse();
    cJSON_AddItemToArray(features, f_false);
    cJSON *f_null = cJSON_CreateNull();
    cJSON_AddItemToArray(features, f_null);
    cJSON *dup_false = cJSON_Duplicate(f_false, (cJSON_bool)0);
    cJSON_AddItemToObject(settings, "dup_false", dup_false);

    // step 3: Operate and Validate
    cJSON *retrieved_features = cJSON_GetObjectItem(settings, "features");
    int feature_count = cJSON_GetArraySize(retrieved_features);
    cJSON_AddNumberToObject(root, "feature_count", (double)feature_count);
    cJSON *retrieved_dup = cJSON_GetObjectItem(settings, "dup_false");
    cJSON_bool dup_is_false = cJSON_IsFalse(retrieved_dup);
    cJSON_AddNumberToObject(root, "dup_is_false", (double)dup_is_false);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}