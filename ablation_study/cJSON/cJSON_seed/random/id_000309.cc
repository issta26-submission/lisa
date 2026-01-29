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
//<ID> 309
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
    cJSON *first_item = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    char *printed = (char *)0;
    const char *parse_end = (const char *)0;
    cJSON_bool require_null = (cJSON_bool)1;
    double numeric_value = 0.0;
    char json_buf[] = "{\"arr\": [ true, 42, \"text\" ], \"to_delete\": 99}";

    // step 2: Initialize (parse JSON into cJSON structure)
    root = cJSON_ParseWithOpts(json_buf, &parse_end, require_null);

    // step 3: Configure (locate array and create a false item)
    arr = cJSON_GetObjectItem(root, "arr");
    false_item = cJSON_CreateFalse();

    // step 4: Operate (access array element, append false, and serialize)
    first_item = cJSON_GetArrayItem(arr, 0);
    cJSON_AddItemToArray(arr, false_item);
    printed = cJSON_PrintUnformatted(arr);

    // step 5: Validate / modify (use a numeric read and remove an object member)
    numeric_value = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1));
    cJSON_DeleteItemFromObject(root, "to_delete");
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}