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
//<ID> 1352
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "settings", settings);

    // step 2: Configure
    cJSON *thr = cJSON_AddNumberToObject(settings, "threshold", 3.14);
    cJSON *limit = cJSON_AddNumberToObject(settings, "limit", 10.0);

    // step 3: Operate and Validate
    cJSON *new_thr = cJSON_CreateNumber(6.28);
    cJSON_bool replaced = cJSON_ReplaceItemInObject(settings, "threshold", new_thr);
    double thr_val = cJSON_GetNumberValue(cJSON_GetObjectItem(settings, "threshold"));
    double limit_val = cJSON_GetNumberValue(limit);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = (char)('T');
    ((char *)scratch)[1] = (char)('0' + ((int)thr_val % 10));
    ((char *)scratch)[2] = (char)('0' + ((int)limit_val % 10));
    ((char *)scratch)[3] = (char)('0' + (replaced ? 1 : 0));

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}