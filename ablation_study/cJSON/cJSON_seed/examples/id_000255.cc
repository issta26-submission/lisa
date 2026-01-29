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
//<ID> 255
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    const char *labels_arr[] = { "alpha", "beta", "gamma" };
    cJSON *labels = cJSON_CreateStringArray(labels_arr, 3);
    cJSON *v_old = cJSON_CreateNumber(1.0);
    cJSON_AddItemToObjectCS(meta, "version", v_old);
    cJSON_AddItemToObjectCS(meta, "tags", labels);
    cJSON_AddItemToObjectCS(root, "meta", meta);

    // step 2: Configure
    cJSON *v_new = cJSON_CreateNumber(2.71828);
    cJSON_ReplaceItemViaPointer(meta, v_old, v_new);
    cJSON_bool has_version = cJSON_HasObjectItem(meta, "version");
    (void)has_version;

    // step 3: Operate & Validate
    cJSON *version_item = cJSON_GetObjectItem(meta, "version");
    double version_val = cJSON_GetNumberValue(version_item);
    cJSON *version_copy = cJSON_CreateNumber(version_val);
    cJSON_AddItemToObjectCS(root, "version_copy", version_copy);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}