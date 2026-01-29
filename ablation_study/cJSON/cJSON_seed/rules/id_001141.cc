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
//<ID> 1141
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *child_old = NULL;
    cJSON *replacement = NULL;
    cJSON *parsed = NULL;
    cJSON *name_item = NULL;
    char *printed = NULL;
    char *dynamic_str = NULL;
    cJSON_bool is_name_string = 0;
    cJSON_bool is_repl_string = 0;
    const char json[] = "{\"name\":\"original\",\"nested\":{\"val\":1}}";

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    dynamic_str = (char *)cJSON_malloc(9);
    memset(dynamic_str, 0, 9);
    dynamic_str[0] = 'd';
    dynamic_str[1] = 'y';
    dynamic_str[2] = 'n';
    dynamic_str[3] = 'a';
    dynamic_str[4] = 'm';
    dynamic_str[5] = 'i';
    dynamic_str[6] = 'c';
    dynamic_str[7] = '\0';
    child_old = cJSON_CreateString("temp_value");
    cJSON_AddItemToObject(root, "field", child_old);
    replacement = cJSON_CreateString(dynamic_str);
    parsed = cJSON_Parse(json);

    // step 3: Operate / Validate
    cJSON_ReplaceItemViaPointer(root, child_old, replacement);
    name_item = cJSON_GetObjectItem(parsed, "name");
    is_name_string = cJSON_IsString(name_item);
    is_repl_string = cJSON_IsString(replacement);
    printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    cJSON_free(printed);
    cJSON_free(dynamic_str);
    (void)is_name_string;
    (void)is_repl_string;
    // API sequence test completed successfully
    return 66;
}