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
//<ID> 806
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *raw_json = "[10,20,30]";
    cJSON *raw = NULL;
    char *printed = NULL;
    cJSON *parsed_array = NULL;
    int arr_size = 0;
    cJSON *second_item = NULL;
    cJSON *expected_value = NULL;
    cJSON_bool are_equal = 0;
    cJSON *root = NULL;

    // step 2: Setup / Configure
    raw = cJSON_CreateRaw(raw_json);
    printed = cJSON_Print(raw);
    parsed_array = cJSON_Parse(raw_json);
    arr_size = cJSON_GetArraySize(parsed_array);
    second_item = cJSON_GetArrayItem(parsed_array, 1);
    expected_value = cJSON_CreateNumber(20.0);

    // step 3: Operate / Validate
    are_equal = cJSON_Compare(second_item, expected_value, 1);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "numbers", parsed_array);
    (void)arr_size;
    (void)are_equal;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(raw);
    cJSON_Delete(expected_value);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}