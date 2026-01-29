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
//<ID> 352
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *fruits[] = { "apple", "banana", "cherry" };
    cJSON *str_array = cJSON_CreateStringArray(fruits, 3);
    cJSON *root = cJSON_CreateObject();
    cJSON *parsed = NULL;
    cJSON *parsed_arr = NULL;
    cJSON *first_item = NULL;
    char *printed = NULL;
    char buffer[8];
    cJSON_bool first_is_string = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "fruits", str_array);
    memset(buffer, 0, sizeof(buffer));
    printed = cJSON_PrintUnformatted(root);

    // step 3: Operate / Validate
    parsed = cJSON_ParseWithLength(printed, (size_t)strlen(printed));
    parsed_arr = cJSON_GetObjectItem(parsed, "fruits");
    first_item = cJSON_GetArrayItem(parsed_arr, 0);
    first_is_string = cJSON_IsString(first_item);
    const char *first_str = cJSON_GetStringValue(first_item);
    buffer[0] = (char)('0' + (int)first_is_string);
    buffer[1] = first_str ? first_str[0] : '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}