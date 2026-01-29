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
//<ID> 305
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *numbers_arr = (cJSON *)0;
    cJSON *second_item = (cJSON *)0;
    cJSON *injected_false = (cJSON *)0;
    char *printed = (char *)0;
    const char *parse_end = (const char *)0;
    double second_value = 0.0;
    char json_buf[] = "{ \"numbers\": [ 10, 20, 30 ], \"meta\": \"ok\" }";

    // step 2: Initialize / Parse
    root = cJSON_ParseWithOpts(json_buf, &parse_end, (cJSON_bool)1);

    // step 3: Configure - access array and item
    numbers_arr = cJSON_GetObjectItem(root, "numbers");
    second_item = cJSON_GetArrayItem(numbers_arr, 1);
    second_value = cJSON_GetNumberValue(second_item);

    // step 4: Operate - produce an unformatted print and inject a false item
    printed = cJSON_PrintUnformatted(root);
    injected_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "injected_false", injected_false);

    // step 5: Validate - remove the injected item from the object
    cJSON_DeleteItemFromObject(root, "injected_false");

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}