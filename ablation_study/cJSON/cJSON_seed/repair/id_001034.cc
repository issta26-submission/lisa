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
//<ID> 1034
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
    cJSON *name = cJSON_CreateString("initial");
    cJSON_AddItemToObject(config, "name", name);
    cJSON *active = cJSON_CreateTrue();
    cJSON_AddItemToObject(config, "active", active);

    // step 2: Configure
    char *old_name_ptr = cJSON_SetValuestring(name, "updated");
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(config, "meta", meta);
    cJSON *version = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(meta, "version", version);

    // step 3: Operate and Validate
    cJSON_bool has_name = cJSON_HasObjectItem(config, "name");
    cJSON *has_name_bool = cJSON_CreateBool(has_name);
    cJSON_AddItemToObject(root, "has_name", has_name_bool);
    cJSON_bool active_is_bool = cJSON_IsBool(active);
    cJSON_bool active_is_false = cJSON_IsFalse(active);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = (char)('0' + (int)active_is_bool);
    ((char *)scratch)[1] = (char)('0' + (int)active_is_false);
    ((char *)scratch)[2] = (char)('0' + (int)has_name);
    ((char *)scratch)[3] = (char)('0' + (int)(old_name_ptr != NULL));
    char *flat = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(flat);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}