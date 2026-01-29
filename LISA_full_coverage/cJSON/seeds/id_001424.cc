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
//<ID> 1424
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"ints\":[1,2,3],\"meta\":{\"flag\":true}}";
    cJSON *root = cJSON_Parse(json_text);
    cJSON *ints = cJSON_GetObjectItem(root, "ints");
    cJSON *first_elem = cJSON_GetArrayItem(ints, 0);
    double first_value = cJSON_GetNumberValue(first_elem);

    // step 2: Configure
    cJSON *new_number = cJSON_CreateNumber(99.5);
    cJSON_AddItemToArray(ints, new_number);
    cJSON *appended_elem = cJSON_GetArrayItem(ints, 3);
    cJSON_bool appended_invalid = cJSON_IsInvalid(appended_elem);

    // step 3: Operate & Validate
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buffer = (char *)cJSON_malloc(printed_len + 1);
    memset(buffer, 0, printed_len + 1);
    memcpy(buffer, printed, printed_len + 1);
    cJSON_Minify(buffer);
    cJSON_free(printed);
    double appended_value = cJSON_GetNumberValue(appended_elem);
    (void)first_value;
    (void)appended_invalid;
    (void)appended_value;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}