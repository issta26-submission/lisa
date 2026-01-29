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
//<ID> 1357
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *initial = cJSON_AddNumberToObject(root, "value", 3.14);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateNumber(2.718);
    cJSON_bool replaced = cJSON_ReplaceItemInObject(root, "value", replacement);
    cJSON *extra = cJSON_AddNumberToObject(root, "extra", 100.0);

    // step 3: Operate and Validate
    double current_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "value"));
    double extra_value = cJSON_GetNumberValue(extra);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = (char)('0' + ((int)current_value % 10));
    ((char *)scratch)[1] = (char)('0' + ((int)extra_value % 10));
    ((char *)scratch)[2] = (char)('0' + (replaced ? 1 : 0));

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}