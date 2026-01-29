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
//<ID> 197
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *ver = cJSON_CreateNumber(1.0);
    cJSON_AddItemToObject(root, "version", ver);
    cJSON *name = cJSON_CreateString("example_name");
    cJSON_AddItemToObject(root, "name", name);
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *threshold = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(config, "threshold", threshold);

    // step 2: Configure
    char *unformatted = cJSON_PrintUnformatted(root);
    cJSON_Minify(unformatted);
    cJSON *new_ver = cJSON_CreateNumber(2.5);
    cJSON_bool replace_ok = cJSON_ReplaceItemInObjectCaseSensitive(root, "version", new_ver);

    // step 3: Operate and Validate
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(root, "name");
    cJSON_bool name_is_str = cJSON_IsString(name_item);
    cJSON *ver_item = cJSON_GetObjectItemCaseSensitive(root, "version");
    double ver_val = cJSON_GetNumberValue(ver_item);
    double thr_val = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(config, "threshold"));
    int summary = (int)name_is_str + (int)ver_val + (int)thr_val + (int)replace_ok + (root ? root->type : 0);
    (void)summary;

    // step 4: Cleanup
    cJSON_free(unformatted);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}