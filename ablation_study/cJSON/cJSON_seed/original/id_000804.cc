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
//<ID> 804
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *num_item = cJSON_CreateNumber(123.0);
    cJSON *str_item = cJSON_CreateString("example");

    // step 2: Configure
    cJSON_AddItemToArray(arr, true_item);
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddRawToObject(meta, "blob", "{\"x\":42,\"flag\":true}");
    cJSON_AddNumberToObject(meta, "count", (double)cJSON_GetArraySize(arr));

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON *retrieved_blob = cJSON_GetObjectItemCaseSensitive(meta, "blob");
    cJSON *retrieved_items = cJSON_GetObjectItemCaseSensitive(root, "items");
    cJSON *first_item = cJSON_GetArrayItem(retrieved_items, 0);
    cJSON_bool first_is_true = cJSON_IsTrue(first_item);
    (void)retrieved_blob;
    (void)first_is_true;

    // step 4: Validate & Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}