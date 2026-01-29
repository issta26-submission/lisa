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
//<ID> 1420
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"arr\":[10,20,30],\"meta\":{\"valid\":true}}";
    cJSON *root = cJSON_Parse(json_text);
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    cJSON *second_elem = cJSON_GetArrayItem(arr, 1);

    // step 2: Configure
    cJSON *new_num = cJSON_CreateNumber(99.5);
    cJSON_ReplaceItemInArray(arr, 1, new_num);
    cJSON *replaced_elem = cJSON_GetArrayItem(arr, 1);

    // step 3: Operate & Validate
    cJSON_bool invalid_before = cJSON_IsInvalid(second_elem);
    cJSON_bool invalid_after = cJSON_IsInvalid(replaced_elem);
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);
    (void)invalid_before;
    (void)invalid_after;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}