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
//<ID> 1425
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"items\":[10,20,30],\"meta\":{\"name\":\"test\"}}";
    cJSON *root = cJSON_Parse(json);
    cJSON *items = cJSON_GetObjectItem(root, "items");
    cJSON *second_item = cJSON_GetArrayItem(items, 1);
    double second_value = cJSON_GetNumberValue(second_item);
    cJSON_bool second_invalid = cJSON_IsInvalid(second_item);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateNumber(second_value * 2.0);
    cJSON_ReplaceItemInArray(items, 2, replacement);
    cJSON *append_num = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToArray(items, append_num);

    // step 3: Operate & Validate
    cJSON *third_item = cJSON_GetArrayItem(items, 2);
    double third_value = cJSON_GetNumberValue(third_item);
    cJSON_bool third_invalid = cJSON_IsInvalid(third_item);
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    (void)second_value;
    (void)third_value;
    (void)second_invalid;
    (void)third_invalid;
    cJSON_free(printed);
    cJSON_free(buf);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}