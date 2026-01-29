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
//<ID> 1423
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddTrueToObject(meta, "enabled");
    cJSON_AddNumberToObject(meta, "version", 2.5);
    cJSON *values = cJSON_AddArrayToObject(root, "values");
    cJSON_AddItemToArray(values, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(20.0));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(30.0));

    // step 2: Configure
    int count = cJSON_GetArraySize(values);
    cJSON_AddNumberToObject(meta, "count", (double)count);

    // step 3: Operate and Validate
    cJSON *enabled_item = cJSON_GetObjectItem(meta, "enabled");
    cJSON_bool enabled_true = cJSON_IsTrue(enabled_item);
    cJSON *count_item = cJSON_GetObjectItem(meta, "count");
    double count_val = cJSON_GetNumberValue(count_item);
    char *render = cJSON_PrintUnformatted(root);
    char *buf = (char *)cJSON_malloc(64);
    memset(buf, 0, 64);
    buf[0] = render ? render[0] : '0';
    buf[1] = (char)('0' + (count % 10));
    buf[2] = (char)('0' + (int)enabled_true);
    buf[3] = (char)('A' + (int)count_val % 26);

    // step 4: Cleanup
    cJSON_free(buf);
    cJSON_free(render);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}