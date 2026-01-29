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
//<ID> 788
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json = "{\"numbers\": [1, 2, 3], \"status\": \"ok\"}";
    cJSON *parsed = NULL;
    cJSON *numbers = NULL;
    cJSON *old_item = NULL;
    double old_val = 0.0;
    cJSON *new_num = NULL;
    cJSON *root = NULL;
    cJSON *copied_array = NULL;
    cJSON *first_elem = NULL;
    cJSON *first_elem_number = NULL;
    char *printed = NULL;

    // step 2: Setup / Configure
    parsed = cJSON_Parse(json);
    numbers = cJSON_GetObjectItem(parsed, "numbers");
    old_item = cJSON_GetArrayItem(numbers, 1);
    old_val = cJSON_GetNumberValue(old_item);
    new_num = cJSON_CreateNumber(old_val * 10.0);
    cJSON_ReplaceItemInArray(numbers, 1, new_num);
    root = cJSON_CreateObject();
    copied_array = cJSON_Duplicate(numbers, 1);
    cJSON_AddItemToObject(root, "numbers_copy", copied_array);
    first_elem = cJSON_GetArrayItem(copied_array, 0);
    first_elem_number = cJSON_CreateNumber(cJSON_GetNumberValue(first_elem));
    cJSON_AddItemToObject(root, "first_elem", first_elem_number);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}