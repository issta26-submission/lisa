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
//<ID> 1370
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddStringToObject(root, "status", "ok");

    // step 2: Configure
    cJSON *child = cJSON_CreateObject();
    cJSON_AddNumberToObject(child, "value", 42.0);
    cJSON_AddItemToObject(root, "child", child);
    cJSON_bool child_present = cJSON_HasObjectItem(root, "child");
    cJSON_AddBoolToObject(root, "child_exists", child_present);
    cJSON *dup_child = cJSON_Duplicate(child, 1);

    // step 3: Operate
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_ReplaceItemInObjectCaseSensitive(root, "status", replacement);
    char *printed = cJSON_PrintBuffered(root, 256, 1);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);

    // step 4: Validate & Cleanup
    cJSON_Delete(dup_child);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}