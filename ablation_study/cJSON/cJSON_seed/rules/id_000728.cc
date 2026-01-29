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
//<ID> 728
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
    cJSON *meta = NULL;
    cJSON *flag_true = NULL;
    cJSON *new_flag = NULL;
    cJSON *inner_initial = NULL;
    cJSON *inner_ptr = NULL;
    cJSON *replacement_str = NULL;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON_bool replace_ok1 = 0;
    cJSON_bool replace_ok2 = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    meta = cJSON_CreateObject();
    flag_true = cJSON_CreateTrue();
    add_ok1 = cJSON_AddItemToObject(root, "flag", flag_true);
    add_ok2 = cJSON_AddItemToObject(root, "meta", meta);
    inner_initial = cJSON_CreateString("initial");
    cJSON_AddItemToObject(meta, "inner", inner_initial);

    // step 3: Operate / Validate
    cJSON *got_flag = cJSON_GetObjectItem(root, "flag");
    new_flag = cJSON_CreateTrue();
    replace_ok1 = cJSON_ReplaceItemInObjectCaseSensitive(root, "flag", new_flag);
    inner_ptr = cJSON_GetObjectItem(meta, "inner");
    replacement_str = cJSON_CreateString("replaced");
    replace_ok2 = cJSON_ReplaceItemViaPointer(meta, inner_ptr, replacement_str);
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