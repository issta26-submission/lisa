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
//<ID> 353
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *strings[] = { "alpha", "beta", "gamma" };
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    cJSON *arr = cJSON_CreateStringArray(strings, 3);
    cJSON *root = cJSON_CreateObject();
    cJSON *parsed = NULL;
    char *printed = NULL;
    cJSON *retrieved_arr = NULL;
    cJSON *second_item = NULL;
    const char *second_str = NULL;
    cJSON_bool is_second_string = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "items", arr);
    printed = cJSON_PrintUnformatted(root);

    // step 3: Operate / Validate
    size_t printed_len = strlen(printed);
    parsed = cJSON_ParseWithLength(printed, printed_len);
    retrieved_arr = cJSON_GetObjectItem(parsed, "items");
    second_item = cJSON_GetArrayItem(retrieved_arr, 1);
    is_second_string = cJSON_IsString(second_item);
    second_str = cJSON_GetStringValue(second_item);
    buffer[0] = (char)('0' + (int)is_second_string);
    buffer[1] = second_str ? second_str[0] : '\0';
    buffer[2] = printed[0];

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}