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
//<ID> 191
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *version = cJSON_CreateNumber(1.0);
    cJSON_AddItemToObject(root, "version", version);
    cJSON *title = cJSON_CreateString("Test Title");
    cJSON_AddItemToObject(root, "title", title);
    cJSON *mode = cJSON_CreateString("alpha");
    cJSON_AddItemToObject(root, "mode", mode);

    // step 2: Configure
    char *json_str = cJSON_PrintUnformatted(root);
    cJSON_Minify(json_str);
    void *tmp_buf = cJSON_malloc(64);
    memset(tmp_buf, 0, 64);

    // step 3: Operate and Validate
    cJSON *mode_before = cJSON_GetObjectItemCaseSensitive(root, "mode");
    cJSON *new_mode = cJSON_CreateNumber(2.0);
    cJSON_bool replaced = cJSON_ReplaceItemInObjectCaseSensitive(root, "mode", new_mode);
    cJSON *mode_after = cJSON_GetObjectItemCaseSensitive(root, "mode");
    double mode_val = cJSON_GetNumberValue(mode_after);
    cJSON *version_item = cJSON_GetObjectItemCaseSensitive(root, "version");
    double version_val = cJSON_GetNumberValue(version_item);
    cJSON *title_item = cJSON_GetObjectItemCaseSensitive(root, "title");
    cJSON_bool title_is_string = cJSON_IsString(title_item);
    int summary = (int)mode_val + (int)version_val + (int)replaced + (int)title_is_string + (root ? root->type : 0) + (mode_before ? mode_before->type : 0);
    (void)summary;

    // step 4: Cleanup
    cJSON_free(json_str);
    cJSON_free(tmp_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}