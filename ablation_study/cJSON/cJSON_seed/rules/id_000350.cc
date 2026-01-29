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
//<ID> 350
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *strings[] = { "alpha", "beta", "gamma" };
    int count = 3;
    cJSON *string_array = cJSON_CreateStringArray(strings, count);
    cJSON *root = cJSON_CreateObject();
    cJSON *parsed = NULL;
    cJSON *parsed_arr = NULL;
    cJSON *first_item = NULL;
    cJSON_bool first_is_string = 0;
    char *printed = NULL;
    size_t printed_len = 0;
    char *scratch = (char *)cJSON_malloc(64);
    memset(scratch, 0, 64);
    const char *first_value = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "items", string_array);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    printed_len = strlen(printed);
    parsed = cJSON_ParseWithLength(printed, printed_len);
    parsed_arr = cJSON_GetObjectItem(parsed, "items");
    first_item = cJSON_GetArrayItem(parsed_arr, 0);
    first_is_string = cJSON_IsString(first_item);
    first_value = cJSON_GetStringValue(first_item);
    scratch[0] = (char)('0' + (int)first_is_string);
    scratch[1] = first_value[0];
    scratch[2] = printed[0];

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}