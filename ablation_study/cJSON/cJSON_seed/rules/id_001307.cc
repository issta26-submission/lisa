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
//<ID> 1307
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json = "{\"arr\":[true,42],\"flag\":true}";
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *item0 = NULL;
    cJSON *item1 = NULL;
    cJSON *detached = NULL;
    char *printed = NULL;
    cJSON_bool is_true = 0;
    double number_after_detach = 0.0;

    // step 2: Setup / Configure
    root = cJSON_Parse(json);
    arr = cJSON_GetObjectItem(root, "arr");
    item0 = cJSON_GetArrayItem(arr, 0);
    item1 = cJSON_GetArrayItem(arr, 1);

    // step 3: Operate / Validate
    is_true = cJSON_IsTrue(item0);
    detached = cJSON_DetachItemViaPointer(arr, item0);
    number_after_detach = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    printed = cJSON_PrintUnformatted(root);
    (void)is_true;
    (void)number_after_detach;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}