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
//<ID> 1309
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
    cJSON *first = NULL;
    cJSON *shifted = NULL;
    cJSON *detached = NULL;
    cJSON *marker = NULL;
    cJSON_bool first_was_true = 0;
    const char *json = "{\"arr\": [true, 123.0, \"three\"]}";

    // step 2: Setup / Configure
    root = cJSON_Parse(json);
    arr = cJSON_GetObjectItem(root, "arr");
    first = cJSON_GetArrayItem(arr, 0);
    first_was_true = cJSON_IsTrue(first);
    marker = cJSON_CreateBool(first_was_true);
    cJSON_AddItemToObject(root, "first_was_true", marker);

    // step 3: Operate / Validate
    detached = cJSON_DetachItemViaPointer(arr, first);
    shifted = cJSON_GetArrayItem(arr, 0);
    (void)shifted;
    (void)first_was_true;

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(detached);
    // API sequence test completed successfully
    return 66;
}