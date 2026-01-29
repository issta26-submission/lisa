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
//<ID> 466
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"arr\":[true,false,123,\"text\"]}";
    cJSON *root = cJSON_Parse(json);

    // step 2: Configure
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    cJSON *new_false = cJSON_CreateFalse();
    cJSON_InsertItemInArray(arr, 2, new_false);
    cJSON *inserted = cJSON_GetArrayItem(arr, 2);
    cJSON_bool was_false = cJSON_IsFalse(inserted);

    // step 3: Operate
    char *out = cJSON_Print(root);
    cJSON_free(out);

    // step 4: Validate and Cleanup
    (void)was_false;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}