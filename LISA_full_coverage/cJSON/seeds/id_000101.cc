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
//<ID> 101
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
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "name", "synth");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *flag = cJSON_AddTrueToObject(root, "enabled");
    cJSON *num = cJSON_CreateNumber(7.5);
    cJSON_AddItemToArray(arr, num);
    cJSON *first = cJSON_CreateString("first");
    cJSON_AddItemToArray(arr, first);
    cJSON *inserted = cJSON_CreateString("inserted");
    cJSON_InsertItemInArray(arr, 1, inserted);

    // step 3: Operate and Validate
    char *printed = cJSON_Print(root);
    cJSON_Minify(printed);
    int arr_size = cJSON_GetArraySize(arr);
    int summary = arr_size + (printed ? (int)printed[0] : 0);
    (void)summary;
    (void)flag;

    // step 4: Cleanup
    if (printed) cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}