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
//<ID> 1202
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json = "{\"flag\":false}";
    size_t json_len = strlen(json);
    cJSON *root = NULL;
    cJSON *flag = NULL;
    cJSON *true_item = NULL;
    char *printed = NULL;
    char buffer[128];
    cJSON_bool was_false = 0;
    cJSON_bool replace_result = 0;
    cJSON_bool ref_result = 0;

    // step 2: Setup / Configure
    root = cJSON_ParseWithLength(json, json_len);
    flag = cJSON_GetObjectItem(root, "flag");
    was_false = cJSON_IsFalse(flag);
    true_item = cJSON_CreateTrue();
    replace_result = cJSON_ReplaceItemViaPointer(root, flag, true_item);
    ref_result = cJSON_AddItemReferenceToObject(root, "flag_ref", true_item);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    memset(buffer, 0, sizeof(buffer));
    strncpy(buffer, printed ? printed : "", sizeof(buffer) - 1);
    (void)was_false;
    (void)replace_result;
    (void)ref_result;
    (void)buffer;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}