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
//<ID> 618
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON *num1 = cJSON_CreateNumber(42.0);
    cJSON *num2 = cJSON_CreateNumber(58.5);
    cJSON *shared_num = cJSON_CreateNumber(77.7);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToArray(arr, shared_num);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(meta, "value", 100.0);
    cJSON *refs = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "refs", refs);

    // step 2: Configure
    cJSON *meta_value = cJSON_GetObjectItem(meta, "value");
    double meta_val = cJSON_GetNumberValue(meta_value);
    (void)meta_val;
    cJSON_AddItemReferenceToArray(refs, shared_num);

    // step 3: Operate
    cJSON *detached_meta = cJSON_DetachItemFromObjectCaseSensitive(root, "meta");
    cJSON *detached_value = cJSON_GetObjectItem(detached_meta, "value");
    double detached_val = cJSON_GetNumberValue(detached_value);
    (void)detached_val;
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObject(root, "refs");
    cJSON_DeleteItemFromObject(root, "arr");
    cJSON_Delete(detached_meta);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}