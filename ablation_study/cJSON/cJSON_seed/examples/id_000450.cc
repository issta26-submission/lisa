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
//<ID> 450
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version_str = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *numbers = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", numbers);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "version", cJSON_CreateString(version_str));

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(3.1415);
    cJSON_AddItemToArray(numbers, n1);
    cJSON *n2 = cJSON_CreateNumber(2.718);
    cJSON_AddItemToArray(numbers, n2);
    cJSON *n3 = cJSON_CreateNumber(1.618);
    cJSON_AddItemToArray(numbers, n3);
    cJSON *parsed = cJSON_Parse("{\"inner\":{\"x\":7,\"y\":8}}");
    cJSON_AddItemToObject(root, "parsed", parsed);

    // step 3: Operate & Validate
    double v0 = cJSON_GetNumberValue(cJSON_GetArrayItem(numbers, 0));
    double v1 = cJSON_GetNumberValue(cJSON_GetArrayItem(numbers, 1));
    double v2 = cJSON_GetNumberValue(cJSON_GetArrayItem(numbers, 2));
    double total = v0 + v1 + v2;
    char status_buf[128];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "sum=%.3f", total);
    cJSON_AddStringToObject(root, "status", status_buf);
    cJSON_AddItemToObject(meta, "sum", cJSON_CreateNumber(total));

    // step 4: Cleanup
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapshot_str = cJSON_CreateString(snapshot);
    cJSON_AddItemToObject(root, "snapshot", snapshot_str);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}