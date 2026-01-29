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
//<ID> 622
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
    cJSON *arr = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("one");
    cJSON *s2 = cJSON_CreateString("two");
    cJSON *child = NULL;
    cJSON *repl = NULL;
    const cJSON *got_child = NULL;
    const cJSON *got_label = NULL;
    const char *label_val = NULL;
    size_t root_len = 0;
    size_t to_copy = 0;
    size_t label_len = 0;
    size_t to_copy2 = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToObject(root, "values", arr);
    child = cJSON_AddObjectToObject(root, "child");
    cJSON_AddItemToObject(child, "label", cJSON_CreateString("example"));

    // step 3: Operate / Validate
    got_child = cJSON_GetObjectItem(root, "child");
    got_label = cJSON_GetObjectItem(got_child, "label");
    label_val = cJSON_GetStringValue(got_label);
    printed_root = cJSON_PrintUnformatted(root);
    root_len = printed_root ? strlen(printed_root) : 0;
    to_copy = root_len < 511 ? root_len : 511;
    memcpy(scratch, printed_root ? printed_root : "", to_copy);
    scratch[to_copy] = '\0';
    label_len = label_val ? strlen(label_val) : 0;
    to_copy2 = (label_len < (511 - to_copy)) ? label_len : (511 - to_copy);
    memcpy(scratch + to_copy, label_val ? label_val : "", to_copy2);
    scratch[to_copy + to_copy2] = '\0';
    repl = cJSON_CreateString("replaced_two");
    cJSON_ReplaceItemInArray(arr, 1, repl);

    // step 4: Cleanup
    cJSON_free(printed_root);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}