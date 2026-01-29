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
//<ID> 612
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON *n2 = cJSON_CreateNumber(2.71);
    cJSON_AddItemToArray(data, n1);
    cJSON_AddItemToArray(data, n2);
    cJSON_AddItemToObject(root, "data", data);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddNumberToObject(meta, "count", 2);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *data_ref = cJSON_GetObjectItem(root, "data");
    cJSON *first = cJSON_GetArrayItem(data_ref, 0);
    cJSON *meta_ref = cJSON_GetObjectItem(root, "meta");
    cJSON *count_item = cJSON_GetObjectItem(meta_ref, "count");
    double count_val = cJSON_GetNumberValue(count_item);

    // step 3: Operate
    cJSON *detached_data = cJSON_DetachItemFromObjectCaseSensitive(root, "data");
    cJSON_AddItemReferenceToArray(detached_data, first);
    cJSON_DeleteItemFromObject(root, "meta");
    char *printed = cJSON_PrintUnformatted(detached_data);

    // step 4: Validate & Cleanup
    double first_val = cJSON_GetNumberValue(first);
    double summary = first_val + count_val;
    cJSON_AddNumberToObject(root, "summary", summary);
    if (printed) { cJSON_free(printed); }
    cJSON_Delete(detached_data);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}