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
//<ID> 1318
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *num = NULL;
    cJSON *raw = NULL;
    cJSON *parsed = NULL;
    cJSON *parsed_arr = NULL;
    cJSON *parsed_item_raw = NULL;
    const char *parse_end = NULL;
    char *printed = NULL;
    size_t printed_len = 0;
    cJSON_bool raw_flag = 0;
    cJSON_bool equal_flag = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_AddArrayToObject(root, "arr");
    num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, num);
    raw = cJSON_CreateRaw("unquoted_raw");
    cJSON_AddItemToArray(arr, raw);
    printed = cJSON_PrintUnformatted(root);
    printed_len = printed ? (size_t)strlen(printed) : (size_t)0;

    // step 3: Operate / Validate
    parsed = cJSON_ParseWithLengthOpts(printed, printed_len, &parse_end, 1);
    parsed_arr = cJSON_GetObjectItem(parsed, "arr");
    parsed_item_raw = cJSON_GetArrayItem(parsed_arr, 1);
    raw_flag = cJSON_IsRaw(parsed_item_raw);
    equal_flag = cJSON_Compare(root, parsed, 1);
    cJSON_AddBoolToObject(root, "parsed_raw_was_raw", raw_flag);
    cJSON_AddBoolToObject(root, "equal_to_parsed", equal_flag);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}