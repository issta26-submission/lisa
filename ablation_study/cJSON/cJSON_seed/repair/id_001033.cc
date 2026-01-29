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
//<ID> 1033
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
    cJSON *name = cJSON_CreateString("default");
    cJSON_AddItemToObject(config, "name", name);
    cJSON *flagA = cJSON_CreateTrue();
    cJSON *flagB = cJSON_CreateFalse();
    cJSON_AddItemToObject(config, "flagA", flagA);
    cJSON_AddItemToObject(config, "flagB", flagB);
    cJSON_AddNumberToObject(config, "version", 1.0);

    // step 2: Configure
    char *changed_name = cJSON_SetValuestring(name, "updated");
    (void)changed_name;
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "status", "ok");

    // step 3: Operate and Validate
    cJSON_bool has_flagB = cJSON_HasObjectItem(config, "flagB");
    cJSON *has_flagB_item = cJSON_CreateBool(has_flagB);
    cJSON_AddItemToObject(root, "has_flagB", has_flagB_item);
    cJSON_bool flagB_is_bool = cJSON_IsBool(flagB);
    cJSON_bool flagB_is_false = cJSON_IsFalse(flagB);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    ((char *)scratch)[0] = (char)('0' + (int)flagB_is_bool);
    ((char *)scratch)[1] = (char)('0' + (int)flagB_is_false);
    char *flat = cJSON_PrintUnformatted(root);
    cJSON_free(flat);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}