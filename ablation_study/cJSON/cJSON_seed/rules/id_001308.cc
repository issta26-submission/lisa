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
//<ID> 1308
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
    cJSON *item0 = NULL;
    cJSON *item2 = NULL;
    cJSON *detached = NULL;
    char *printed = NULL;
    double num_val = 0.0;
    cJSON_bool is_true = 0;
    cJSON_bool is_true_detached = 0;
    const char *json = "{\"arr\": [true, false, 42, \"hello\"]}";

    // step 2: Setup / Configure
    root = cJSON_Parse(json);
    arr = cJSON_GetObjectItem(root, "arr");

    // step 3: Operate / Validate
    item0 = cJSON_GetArrayItem(arr, 0);
    is_true = cJSON_IsTrue(item0);
    item2 = cJSON_GetArrayItem(arr, 2);
    num_val = cJSON_GetNumberValue(item2);
    detached = cJSON_DetachItemViaPointer(arr, item0);
    is_true_detached = cJSON_IsTrue(detached);
    printed = cJSON_PrintUnformatted(root);
    (void)is_true;
    (void)num_val;
    (void)is_true_detached;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    cJSON_Delete(detached);
    // API sequence test completed successfully
    return 66;
}