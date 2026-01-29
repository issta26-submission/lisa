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
//<ID> 916
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"meta\":{\"name\":\"original\"},\"numbers\":[10,20]}";
    cJSON *root = cJSON_ParseWithLength(json, sizeof(json) - 1);
    cJSON *meta = cJSON_GetObjectItemCaseSensitive(root, "meta");

    // step 2: Configure
    cJSON *extras = cJSON_AddArrayToObject(root, "extras");
    cJSON *detached_name = cJSON_DetachItemFromObject(meta, "name");
    cJSON_AddItemToArray(extras, detached_name);
    cJSON *numbers = cJSON_GetObjectItemCaseSensitive(root, "numbers");

    // step 3: Operate
    cJSON *first_num = cJSON_GetArrayItem(numbers, 0);
    double first_val = cJSON_GetNumberValue(first_num);
    cJSON *new_num = cJSON_CreateNumber(first_val + 5.5);
    cJSON_AddItemToArray(extras, new_num);
    cJSON *note = cJSON_CreateString("appended-note");
    cJSON_AddItemToArray(extras, note);

    // step 4: Validate & Cleanup
    char *out = cJSON_PrintUnformatted(root);
    void *buf = cJSON_malloc(256);
    memset(buf, 0, 256);
    char *prebuf = (char *)buf;
    cJSON_PrintPreallocated(root, prebuf, 256, 1);
    cJSON_free(out);
    cJSON_free(prebuf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}