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
//<ID> 1304
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
    cJSON *first_item = NULL;
    cJSON *second_item = NULL;
    cJSON *detached_item = NULL;
    char *printed = NULL;
    cJSON_bool first_is_true = 0;
    double second_value = 0.0;
    const char *json_text = "{\"arr\":[true, 42.5, {\"nested\":\"ok\"}], \"flag\": true}";

    // step 2: Setup / Parse JSON
    root = cJSON_Parse(json_text);
    arr = cJSON_GetObjectItem(root, "arr");

    // step 3: Operate / Validate
    first_item = cJSON_GetArrayItem(arr, 0);
    second_item = cJSON_GetArrayItem(arr, 1);
    first_is_true = cJSON_IsTrue(first_item);
    second_value = cJSON_GetNumberValue(second_item);
    detached_item = cJSON_DetachItemViaPointer(arr, first_item);
    printed = cJSON_PrintUnformatted(root);
    (void)first_is_true;
    (void)second_value;
    (void)printed;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    cJSON_Delete(detached_item);
    // API sequence test completed successfully
    return 66;
}