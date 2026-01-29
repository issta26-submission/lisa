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
//<ID> 74
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
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *name = cJSON_CreateString("initial");
    cJSON_AddItemToObject(meta, "name", name);

    // step 2: Configure
    char *updated = cJSON_SetValuestring(name, "updated");
    cJSON_bool has_meta = cJSON_HasObjectItem(root, "meta");
    cJSON_AddNumberToObject(root, "meta_present", (double)has_meta);
    cJSON *retr_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *dup_meta = cJSON_Duplicate(retr_meta, 1);
    cJSON_AddItemToObject(root, "meta_copy", dup_meta);

    // step 3: Operate and Validate
    cJSON *name_item = cJSON_GetObjectItem(retr_meta, "name");
    char *finalstr = cJSON_SetValuestring(name_item, "final");
    cJSON_bool dup_has_name = cJSON_HasObjectItem(dup_meta, "name");
    cJSON_AddNumberToObject(root, "name_exists_flag", (double)dup_has_name);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}