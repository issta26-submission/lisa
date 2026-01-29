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
//<ID> 1037
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);

    // step 2: Configure
    cJSON *enabled = cJSON_CreateBool(1);
    cJSON_AddItemToObject(config, "enabled", enabled);
    cJSON *mode = cJSON_CreateString("initial");
    cJSON_AddItemToObject(config, "mode", mode);
    char *new_mode = cJSON_SetValuestring(mode, "updated");

    // step 3: Operate and Validate
    cJSON_bool has_cfg = cJSON_HasObjectItem(root, "config");
    cJSON *has_cfg_item = cJSON_CreateBool(has_cfg);
    cJSON_AddItemToObject(root, "has_config", has_cfg_item);
    cJSON *got_enabled = cJSON_GetObjectItem(config, "enabled");
    cJSON_bool is_bool = cJSON_IsBool(got_enabled);
    cJSON_bool is_false = cJSON_IsFalse(got_enabled);
    void *scratch = cJSON_malloc(16);
    memset(scratch, 0, 16);
    ((char *)scratch)[0] = (char)('0' + (int)is_bool);
    ((char *)scratch)[1] = (char)('0' + (int)is_false);
    char *flat = cJSON_PrintUnformatted(root);
    cJSON_free(flat);
    cJSON_free(scratch);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    (void)new_mode;
    return 66;
}