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
//<ID> 301
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
    cJSON *item = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    char *printed = (char *)0;
    const char *parse_end = (const char *)0;
    char json[] = "{\"arr\":[false,{\"nested\":3},\"hello\"],\"removeMe\":\"bye\"}";

    // step 2: Initialize (parse JSON with options)
    root = cJSON_ParseWithOpts(json, &parse_end, (cJSON_bool)1);

    // step 3: Configure (locate array and get an element)
    arr = cJSON_GetObjectItem(root, "arr");
    item = cJSON_GetArrayItem(arr, 0);

    // step 4: Operate (print unformatted, add printed string, inject a false)
    printed = cJSON_PrintUnformatted(item);
    cJSON_AddStringToObject(root, "first_item_unformatted", printed);
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "injected_false", false_item);

    // step 5: Validate (remove an object member that was in the original JSON)
    cJSON_DeleteItemFromObject(root, "removeMe");

    // step 6: Cleanup (free printed buffer and delete the whole tree)
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}