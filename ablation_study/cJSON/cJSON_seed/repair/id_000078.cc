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
//<ID> 78
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
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON_AddItemToArray(items, n1);

    // step 2: Configure
    cJSON *root_dup = cJSON_Duplicate(root, 1);
    cJSON *dup_meta = cJSON_GetObjectItem(root_dup, "meta");
    cJSON *dup_name = cJSON_GetObjectItem(dup_meta, "name");
    char *updated = cJSON_SetValuestring(dup_name, "updated");
    cJSON_AddStringToObject(root, "status", "configured");

    // step 3: Operate and Validate
    cJSON_bool has_meta_root = cJSON_HasObjectItem(root, "meta");
    cJSON_bool has_meta_dup = cJSON_HasObjectItem(root_dup, "meta");
    cJSON_AddNumberToObject(root, "has_meta_root", (double)has_meta_root);
    cJSON_AddNumberToObject(root, "has_meta_dup", (double)has_meta_dup);
    cJSON_AddNumberToObject(root, "dup_name_first_char", (double)(unsigned char)updated[0]);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root_dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}