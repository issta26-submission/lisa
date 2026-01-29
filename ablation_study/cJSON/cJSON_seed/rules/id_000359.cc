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
//<ID> 359
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *strings[] = { "alpha", "beta", "gamma" };
    cJSON *strarr = cJSON_CreateStringArray(strings, 3);
    cJSON *root = cJSON_CreateObject();
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    cJSON *parsed = NULL;
    size_t printed_len = 0;
    cJSON *retrieved_arr = NULL;
    cJSON *second_elem = NULL;
    cJSON_bool is_second_string = 0;
    const char *second_value = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "items", strarr);
    printed = cJSON_PrintUnformatted(root);
    printed_len = strlen(printed);
    parsed = cJSON_ParseWithLength(printed, printed_len);

    // step 3: Operate / Validate
    retrieved_arr = cJSON_GetObjectItem(parsed, "items");
    second_elem = cJSON_GetArrayItem(retrieved_arr, 1);
    is_second_string = cJSON_IsString(second_elem);
    second_value = cJSON_GetStringValue(second_elem);
    buffer[0] = (char)('0' + (int)is_second_string);
    buffer[1] = second_value[0];

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}