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
//<ID> 623
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed_root = NULL;
    char *scratch = (char *)cJSON_malloc(512);
    memset(scratch, 0, 512);
    cJSON *root = cJSON_CreateObject();
    cJSON *child1 = cJSON_AddObjectToObject(root, "child1");
    cJSON *arr = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("one");
    cJSON *s2 = cJSON_CreateString("two");
    cJSON *s_repl = NULL;
    cJSON *greeting_item = NULL;
    size_t root_len = 0;
    size_t greeting_len = 0;
    size_t to_copy_root = 0;
    size_t to_copy_greeting = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddItemToObject(child1, "greeting", cJSON_CreateString("hello"));

    // step 3: Operate / Validate
    s_repl = cJSON_CreateString("replaced");
    cJSON_ReplaceItemInArray(arr, 0, s_repl);
    greeting_item = cJSON_GetObjectItem(child1, "greeting");
    printed_root = cJSON_PrintUnformatted(root);
    root_len = printed_root ? strlen(printed_root) : 0;
    greeting_len = cJSON_GetStringValue(greeting_item) ? strlen(cJSON_GetStringValue(greeting_item)) : 0;
    to_copy_root = root_len < 255 ? root_len : 255;
    to_copy_greeting = greeting_len < (511 - to_copy_root) ? greeting_len : (511 - to_copy_root);
    memcpy(scratch, printed_root ? printed_root : "", to_copy_root);
    scratch[to_copy_root] = '\0';
    memcpy(scratch + to_copy_root, cJSON_GetStringValue(greeting_item) ? cJSON_GetStringValue(greeting_item) : "", to_copy_greeting);
    scratch[to_copy_root + to_copy_greeting] = '\0';

    // step 4: Cleanup
    cJSON_free(printed_root);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}