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
//<ID> 1429
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"numbers\":[1,2,3],\"status\":\"ok\"}";
    cJSON *root = cJSON_Parse(json_text);
    cJSON *numbers = cJSON_GetObjectItem(root, "numbers");
    cJSON *first_elem = cJSON_GetArrayItem(numbers, 0);
    cJSON_bool first_is_invalid = cJSON_IsInvalid(first_elem);
    (void)first_is_invalid;

    // step 2: Configure
    cJSON *new_number = cJSON_CreateNumber(4.5);
    cJSON_AddItemToArray(numbers, new_number);
    cJSON *appended_elem = cJSON_GetArrayItem(numbers, 3);
    double appended_value = cJSON_GetNumberValue(appended_elem);
    (void)appended_value;

    // step 3: Operate & Validate
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buffer = (char *)cJSON_malloc(printed_len + 1);
    memset(buffer, 0, printed_len + 1);
    memcpy(buffer, printed, printed_len + 1);
    cJSON_Minify(buffer);
    cJSON_free(printed);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}