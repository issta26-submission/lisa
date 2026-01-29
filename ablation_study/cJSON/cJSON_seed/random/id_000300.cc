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
//<ID> 300
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *parsed = (cJSON *)0;
    cJSON *first_item = (cJSON *)0;
    char *printed = (char *)0;
    const char *parse_end = (const char *)0;
    cJSON_bool first_was_false = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, cJSON_CreateTrue());
    cJSON_AddItemToArray(arr, false_item);
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON_AddNumberToObject(root, "remove_me", 123.0);

    // step 3: Configure
    parsed = cJSON_ParseWithOpts("[ false , true ]", &parse_end, 1);
    first_item = cJSON_GetArrayItem(parsed, 0);

    // step 4: Operate
    printed = cJSON_PrintUnformatted(parsed);
    cJSON_AddStringToObject(root, "parsed_dump", printed);
    cJSON_free(printed);
    cJSON_DeleteItemFromObject(root, "remove_me");

    // step 5: Validate
    first_was_false = cJSON_IsFalse(first_item);
    cJSON_AddBoolToObject(root, "first_is_false", first_was_false);

    // step 6: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}