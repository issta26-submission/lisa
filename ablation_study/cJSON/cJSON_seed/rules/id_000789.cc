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
//<ID> 789
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json = "{\"arr\": [1, 2, 3], \"label\": \"original\"}";
    cJSON *parsed = NULL;
    cJSON *orig_array = NULL;
    cJSON *array_copy = NULL;
    cJSON *root = NULL;
    cJSON *new_number = NULL;
    cJSON *replaced_item = NULL;
    char *printed = NULL;
    double replaced_value = 0.0;

    // step 2: Setup / Configure
    parsed = cJSON_Parse(json);
    orig_array = cJSON_GetObjectItem(parsed, "arr");
    array_copy = cJSON_Duplicate(orig_array, 1);

    // step 3: Operate / Validate
    new_number = cJSON_CreateNumber(42.0);
    cJSON_ReplaceItemInArray(array_copy, 1, new_number);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "modified", array_copy);
    printed = cJSON_PrintUnformatted(root);
    replaced_item = cJSON_GetArrayItem(array_copy, 1);
    replaced_value = cJSON_GetNumberValue(replaced_item);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}