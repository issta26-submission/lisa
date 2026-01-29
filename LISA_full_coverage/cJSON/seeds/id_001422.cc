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
//<ID> 1422
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *constants = cJSON_CreateArray();
    cJSON *num_pi = cJSON_CreateNumber(3.14159);
    cJSON *num_e = cJSON_CreateNumber(2.71828);
    cJSON_AddItemToArray(constants, num_pi);
    cJSON_AddItemToArray(constants, num_e);
    cJSON_AddItemToObject(root, "constants", constants);

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);

    // step 3: Operate & Validate
    cJSON *parsed = cJSON_Parse(printed);
    cJSON *parsed_constants = cJSON_GetObjectItem(parsed, "constants");
    cJSON *first_item = cJSON_GetArrayItem(parsed_constants, 0);
    cJSON_bool first_is_invalid = cJSON_IsInvalid(first_item);
    double first_value = cJSON_GetNumberValue(first_item);
    size_t printed_len = strlen(printed);
    char *buffer = (char *)cJSON_malloc(printed_len + 1);
    memset(buffer, 0, printed_len + 1);
    memcpy(buffer, printed, printed_len + 1);
    cJSON_Minify(buffer);
    cJSON_free(printed);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    (void)first_is_invalid;
    (void)first_value;
    // API sequence test completed successfully
    return 66;
}