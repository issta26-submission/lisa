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
//<ID> 1147
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *old_flag = NULL;
    cJSON *replacement_str = NULL;
    cJSON *parsed = NULL;
    cJSON *parsed_status = NULL;
    cJSON *new_copy = NULL;
    cJSON *check_item = NULL;
    char *printed_json = NULL;
    char *dyn_str = NULL;
    const char *status_text = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    old_flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "status", old_flag);
    dyn_str = (char *)cJSON_malloc(16);
    memset(dyn_str, 0, 16);
    dyn_str[0] = 'O';
    dyn_str[1] = 'K';
    dyn_str[2] = '\0';
    replacement_str = cJSON_CreateString(dyn_str);

    // step 3: Operate / Validate
    cJSON_ReplaceItemViaPointer(root, old_flag, replacement_str);
    check_item = cJSON_GetObjectItem(root, "status");
    cJSON_bool is_string = cJSON_IsString(check_item);
    (void)is_string;
    printed_json = cJSON_Print(root);
    parsed = cJSON_Parse(printed_json);
    parsed_status = cJSON_GetObjectItem(parsed, "status");
    status_text = cJSON_GetStringValue(parsed_status);
    new_copy = cJSON_CreateString(status_text);
    cJSON_AddItemToObject(root, "status_copy", new_copy);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    cJSON_free(printed_json);
    cJSON_free(dyn_str);
    // API sequence test completed successfully
    return 66;
}