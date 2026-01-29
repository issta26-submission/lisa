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
//<ID> 1032
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *cfg = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", cfg);
    cJSON *enabled = cJSON_CreateFalse();
    cJSON_AddItemToObject(cfg, "enabled", enabled);
    cJSON *name = cJSON_CreateString("deviceX");
    cJSON_AddItemToObject(cfg, "name", name);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *ver = cJSON_CreateString(cJSON_Version());
    cJSON_AddItemToObject(meta, "version", ver);

    // step 3: Operate and Validate
    cJSON_bool has_enabled = cJSON_HasObjectItem(cfg, "enabled");
    cJSON_AddItemToObject(root, "has_enabled", cJSON_CreateBool(has_enabled));
    cJSON *got_enabled = cJSON_GetObjectItem(cfg, "enabled");
    cJSON_bool is_bool = cJSON_IsBool(got_enabled);
    cJSON_bool is_false = cJSON_IsFalse(got_enabled);
    cJSON *status = cJSON_CreateString("ok");
    cJSON_AddItemToObject(root, "status", status);
    char *old_status = cJSON_SetValuestring(status, is_false ? "disabled" : "enabled");
    char *flat = cJSON_PrintUnformatted(root);
    cJSON_free(flat);

    // step 4: Cleanup
    void *scratch = cJSON_malloc(16);
    memset(scratch, 0, 16);
    ((char *)scratch)[0] = (char)('0' + (int)is_bool);
    ((char *)scratch)[1] = (char)('0' + (int)is_false);
    ((char *)scratch)[2] = old_status ? old_status[0] : 'X';
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}