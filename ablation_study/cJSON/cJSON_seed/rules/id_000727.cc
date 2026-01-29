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
//<ID> 727
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(256);
    memset(scratch, 0, 256);
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *flag = cJSON_CreateTrue();
    cJSON *flag_replacement = cJSON_CreateTrue();
    cJSON *inner = NULL;
    cJSON *new_inner = NULL;
    char *printed = NULL;
    size_t printed_len = 0;
    cJSON_bool add_ok = 0;
    cJSON_bool replace_ok_case = 0;
    cJSON_bool replace_ok_ptr = 0;

    // step 2: Setup / Configure
    add_ok = cJSON_AddItemToObject(root, "enabled", flag);
    add_ok = cJSON_AddItemToObject(root, "child", child);
    add_ok = cJSON_AddItemToObject(child, "inner", cJSON_CreateTrue());

    // step 3: Operate / Validate
    replace_ok_case = cJSON_ReplaceItemInObjectCaseSensitive(root, "enabled", flag_replacement);
    inner = cJSON_GetObjectItem(child, "inner");
    new_inner = cJSON_CreateFalse();
    replace_ok_ptr = cJSON_ReplaceItemViaPointer(child, inner, new_inner);
    printed = cJSON_PrintUnformatted(root);
    printed_len = printed ? strlen(printed) : 0;
    memcpy(scratch, printed ? printed : "", printed_len);
    scratch[printed_len > 0 ? printed_len : 0] = '\0';
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}