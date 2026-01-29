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
//<ID> 1423
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"numbers\":[10,20,30],\"pi\":3.14159}";
    cJSON *root = cJSON_Parse(json);
    cJSON *nums = cJSON_GetObjectItem(root, "numbers");
    cJSON *first = cJSON_GetArrayItem(nums, 0);
    cJSON_bool first_invalid = cJSON_IsInvalid(first);
    (void)first_invalid;

    // step 2: Configure
    cJSON *new_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(nums, new_item);
    cJSON *fourth = cJSON_GetArrayItem(nums, 3);

    // step 3: Operate & Validate
    double val_first = cJSON_GetNumberValue(first);
    double val_fourth = cJSON_GetNumberValue(fourth);
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);
    (void)val_first;
    (void)val_fourth;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}