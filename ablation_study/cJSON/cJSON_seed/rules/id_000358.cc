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
//<ID> 358
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *values[3] = {"one", "two", "three"};
    cJSON *str_array = cJSON_CreateStringArray(values, 3);
    cJSON *root = cJSON_CreateObject();
    char *printed = NULL;
    size_t printed_len = 0;
    cJSON *parsed = NULL;
    cJSON *parsed_arr = NULL;
    cJSON *item0 = NULL;
    cJSON_bool is_str0 = 0;
    const char *s0 = NULL;
    char *buffer = (char *)cJSON_malloc(32);
    memset(buffer, 0, 32);

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "strings", str_array);
    printed = cJSON_PrintUnformatted(root);
    printed_len = strlen(printed);

    // step 3: Operate / Validate
    parsed = cJSON_ParseWithLength(printed, printed_len);
    parsed_arr = cJSON_GetObjectItem(parsed, "strings");
    item0 = cJSON_GetArrayItem(parsed_arr, 0);
    is_str0 = cJSON_IsString(item0);
    s0 = cJSON_GetStringValue(item0);
    buffer[0] = (char)('0' + (int)is_str0);
    buffer[1] = s0[0];

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}