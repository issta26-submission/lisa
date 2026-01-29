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
//<ID> 720
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
    char *printed = NULL;
    size_t printed_len = 0;
    cJSON *root = NULL;
    cJSON *flag = NULL;
    cJSON *flag2 = NULL;
    cJSON *new_flag = NULL;
    cJSON *replacement = NULL;
    cJSON *item_ptr = NULL;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;
    cJSON_bool replaced_case = 0;
    cJSON_bool replaced_ptr = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    flag = cJSON_CreateTrue();
    added1 = cJSON_AddItemToObject(root, "flag", flag);
    flag2 = cJSON_CreateTrue();
    added2 = cJSON_AddItemToObject(root, "flag2", flag2);

    // step 3: Operate / Validate
    new_flag = cJSON_CreateTrue();
    replaced_case = cJSON_ReplaceItemInObjectCaseSensitive(root, "flag", new_flag);
    item_ptr = cJSON_GetObjectItem(root, "flag2");
    replacement = cJSON_CreateTrue();
    replaced_ptr = cJSON_ReplaceItemViaPointer(root, item_ptr, replacement);
    printed = cJSON_PrintUnformatted(root);
    printed_len = printed ? strlen(printed) : 0;
    memcpy(scratch, printed ? printed : "", printed_len);
    scratch[printed_len] = '\0';
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}