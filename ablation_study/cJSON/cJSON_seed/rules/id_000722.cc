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
//<ID> 722
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(512);
    memset(scratch, 0, 512);
    cJSON *root = cJSON_CreateObject();
    cJSON *child1 = cJSON_CreateObject();
    cJSON *child2 = cJSON_CreateObject();
    cJSON *true1 = cJSON_CreateTrue();
    cJSON *true2 = cJSON_CreateTrue();
    cJSON *tmp = NULL;
    cJSON *old_child = NULL;
    char *printed = NULL;
    size_t printed_len = 0;
    cJSON_bool replaced_flag = 0;
    cJSON_bool replaced_pointer = 0;
    cJSON_bool flag_is_true = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "child", child1);
    cJSON_AddItemToObject(child1, "flag", true1);
    cJSON_AddNumberToObject(child1, "value", 7.0);

    // step 3: Operate / Validate
    replaced_flag = cJSON_ReplaceItemInObjectCaseSensitive(child1, "flag", true2);
    old_child = cJSON_Duplicate(child1, 1);
    replaced_pointer = cJSON_ReplaceItemViaPointer(root, child1, child2);
    cJSON_AddItemToObject(child2, "old_child", old_child);
    tmp = cJSON_GetObjectItem(child2, "old_child");
    tmp = cJSON_GetObjectItem(tmp, "flag");
    flag_is_true = cJSON_IsTrue(tmp);
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