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
//<ID> 1424
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize and Configure
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddNumberToObject(meta, "version", 3.14);
    cJSON_AddTrueToObject(meta, "active");
    cJSON *values = cJSON_AddArrayToObject(root, "values");
    cJSON_AddItemToArray(values, cJSON_CreateNumber(100.0));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(200.0));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(300.0));

    // step 2: Operate
    int count = cJSON_GetArraySize(values);
    cJSON *active_item = cJSON_GetObjectItem(meta, "active");
    cJSON_bool is_active = cJSON_IsTrue(active_item);
    char *unfmt = cJSON_PrintUnformatted(root);

    // step 3: Validate
    char *scratch = (char *)cJSON_malloc(64);
    memset(scratch, 0, 64);
    scratch[0] = unfmt[0];
    scratch[1] = (char)('0' + (count % 10));
    scratch[2] = (char)('0' + (char)is_active);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(unfmt);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}