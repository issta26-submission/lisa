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
//<ID> 355
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *colors[] = { "red", "green", "blue" };
    cJSON *root = cJSON_CreateObject();
    cJSON *str_array = cJSON_CreateStringArray(colors, 3);
    char *printed = NULL;
    size_t printed_len = 0;
    cJSON *parsed = NULL;
    cJSON *parsed_array = NULL;
    cJSON *second_item = NULL;
    cJSON_bool is_second_string = 0;
    const char *second_value = NULL;
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "colors", str_array);
    printed = cJSON_PrintUnformatted(root);
    printed_len = printed ? strlen(printed) : 0;
    parsed = cJSON_ParseWithLength(printed, printed_len);
    parsed_array = cJSON_GetObjectItem(parsed, "colors");

    // step 3: Operate / Validate
    second_item = cJSON_GetArrayItem(parsed_array, 1);
    is_second_string = cJSON_IsString(second_item);
    second_value = cJSON_GetStringValue(second_item);
    buffer[0] = (char)('0' + (int)is_second_string);
    buffer[1] = second_value ? second_value[0] : '\0';
    buffer[2] = printed ? printed[0] : '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}