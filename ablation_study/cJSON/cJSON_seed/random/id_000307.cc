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
//<ID> 307
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
    cJSON *arr_item = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    char *printed = (char *)0;
    const char *parse_end = (const char *)0;
    const char json_text[] = "{\"arr\":[\"first\", true, 3], \"note\":\"initial\"}";

    // step 2: Initialize (parse input JSON)
    root = cJSON_ParseWithOpts(json_text, &parse_end, (cJSON_bool)1);

    // step 3: Configure (locate array and an element, create a false item and attach it)
    arr = cJSON_GetObjectItem(root, "arr");
    arr_item = cJSON_GetArrayItem(arr, 1);
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "to_remove", false_item);

    // step 4: Operate (print an array element unformatted and then remove the added object item)
    printed = cJSON_PrintUnformatted(arr_item);
    cJSON_DeleteItemFromObject(root, "to_remove");

    // step 5: Validate (mark the root to reflect the deletion and preserve printed output into JSON)
    cJSON_AddStringToObject(root, "deleted_marker", "yes");
    cJSON_AddStringToObject(root, "printed_snapshot", printed ? printed : "null");

    // step 6: Cleanup (free allocated string and delete root)
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}