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
//<ID> 675
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", list);
    cJSON *elem0 = cJSON_CreateString("alpha");
    cJSON *elem1 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(list, elem0);
    cJSON_AddItemToArray(list, elem1);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag);

    // step 2: Configure
    cJSON *flag_ptr = cJSON_GetObjectItem(root, "flag");
    cJSON_bool flag_was_bool = cJSON_IsBool(flag_ptr);
    cJSON_AddBoolToObject(root, "flag_was_bool", flag_was_bool);

    // step 3: Operate
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_bool replaced_ok = cJSON_ReplaceItemInObject(root, "flag", replacement);
    (void)replaced_ok;
    cJSON *flag_after = cJSON_GetObjectItem(root, "flag");
    cJSON_bool flag_is_string = cJSON_IsString(flag_after);
    cJSON_AddBoolToObject(root, "flag_is_string", flag_is_string);
    int list_size = cJSON_GetArraySize(list);
    cJSON_AddNumberToObject(root, "list_size", (double)list_size);
    cJSON *first_elem = cJSON_GetArrayItem(list, 0);
    cJSON_bool first_is_string = cJSON_IsString(first_elem);
    cJSON_AddBoolToObject(root, "first_is_string", first_is_string);

    // step 4: Validate & Cleanup
    char buffer[512];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}