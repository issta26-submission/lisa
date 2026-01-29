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
//<ID> 403
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    const char *parse_end = NULL;
    size_t printed_len = 0;
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *bool_item = NULL;
    cJSON *num_item = cJSON_CreateNumber(7.0);
    cJSON *str_item = cJSON_CreateString("seven");
    cJSON *replacement = cJSON_CreateString("eight");
    cJSON *parsed = NULL;
    cJSON *parsed_list = NULL;
    cJSON *first_elem = NULL;
    cJSON_bool replaced = 0;
    cJSON_bool is_enabled = 0;
    double first_val = 0.0;

    // step 2: Setup / Configure
    bool_item = cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, str_item);
    replaced = cJSON_ReplaceItemInArray(arr, 1, replacement);

    // step 3: Operate / Validate
    printed = cJSON_PrintBuffered(root, 256, 1);
    printed_len = (printed != NULL) ? (size_t)strlen(printed) : 0;
    parsed = cJSON_ParseWithLengthOpts(printed, printed_len, &parse_end, 1);
    parsed_list = cJSON_GetObjectItem(parsed, "list");
    first_elem = cJSON_GetArrayItem(parsed_list, 0);
    first_val = cJSON_GetNumberValue(first_elem);
    is_enabled = cJSON_IsTrue(cJSON_GetObjectItem(parsed, "enabled"));
    buffer[0] = (char)('0' + (int)first_val);
    buffer[1] = (char)('0' + (int)is_enabled);
    buffer[2] = (char)('0' + (int)replaced);
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}