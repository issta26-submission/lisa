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
//<ID> 71
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
    cJSON_AddNumberToObject(meta, "first_char_code", (double)(unsigned char)updated[0]);

    // step 3: Operate and Validate
    cJSON *retr_meta = cJSON_GetObjectItem(root, "meta");
    cJSON_bool name_exists = cJSON_HasObjectItem(retr_meta, "name");
    cJSON_AddNumberToObject(root, "name_exists_flag", (double)name_exists);
    cJSON *dup_meta = cJSON_Duplicate(meta, 1);
    cJSON_AddItemToObject(root, "meta_copy", dup_meta);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}