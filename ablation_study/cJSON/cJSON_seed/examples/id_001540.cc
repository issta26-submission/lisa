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
//<ID> 1540
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
    cJSON *name_item = cJSON_CreateString("fuzz-test");
    cJSON_bool added_name = cJSON_AddItemToObject(meta, "name", name_item);
    cJSON_bool added_meta = cJSON_AddItemToObject(root, "meta", meta);
    cJSON *active_item = cJSON_AddBoolToObject(root, "active", 1);
    (void)added_name; (void)added_meta;

    // step 2: Configure
    cJSON_bool has_active = cJSON_HasObjectItem(root, "active");
    cJSON_bool has_meta = cJSON_HasObjectItem(root, "meta");
    cJSON *meta_lookup = cJSON_GetObjectItem(root, "meta");
    cJSON *name_lookup = cJSON_GetObjectItem(meta_lookup, "name");
    char *name_value = cJSON_GetStringValue(name_lookup);

    // step 3: Operate & Validate
    char *printed = cJSON_Print(root);
    int validation_score = (int)has_active + (int)has_meta + (name_value != NULL);
    (void)validation_score;
    (void)active_item;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}