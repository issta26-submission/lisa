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
//<ID> 825
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
    cJSON *data = cJSON_AddObjectToObject(root, "data");
    cJSON *items = cJSON_AddArrayToObject(data, "items");
    cJSON *n0 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(items, n0);
    cJSON_AddItemToArray(items, n2);
    cJSON_AddStringToObject(root, "temp", "transient");

    // step 2: Configure
    cJSON_AddStringToObject(meta, "version", cJSON_Version());
    cJSON *n1 = cJSON_CreateNumber(2.0);
    cJSON_InsertItemInArray(items, 1, n1);

    // step 3: Operate
    char *pretty = cJSON_Print(root);
    char *compact = cJSON_PrintUnformatted(root);
    cJSON_free(pretty);
    cJSON_free(compact);
    cJSON *first = cJSON_GetArrayItem(items, 0);
    double v0 = cJSON_GetNumberValue(first);
    cJSON *middle = cJSON_GetArrayItem(items, 1);
    double v1 = cJSON_GetNumberValue(middle);
    cJSON *sum = cJSON_CreateNumber(v0 + v1);
    cJSON_AddItemToObject(data, "sum", sum);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(root, "temp");
    cJSON *got_ver = cJSON_GetObjectItem(meta, "version");
    const char *ver = cJSON_GetStringValue(got_ver);
    (void)ver;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}