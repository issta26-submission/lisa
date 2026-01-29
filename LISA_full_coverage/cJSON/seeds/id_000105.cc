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
//<ID> 105
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
    cJSON_AddStringToObject(meta, "name", "example");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *flag = cJSON_AddTrueToObject(root, "active");
    cJSON *num = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(arr, num);
    cJSON *str = cJSON_CreateString("one");
    cJSON_AddItemToArray(arr, str);
    cJSON_bool insert_ok = cJSON_InsertItemInArray(arr, 1, cJSON_CreateString("inserted"));

    // step 3: Operate and Validate
    char *printed = cJSON_Print(root);
    cJSON_Minify(printed);
    char *printed2 = cJSON_Print(root);
    int arr_size = cJSON_GetArraySize(arr);
    int summary = (int)insert_ok + arr_size + (int)printed[0];
    (void)flag;
    (void)summary;

    // step 4: Cleanup
    if (printed) cJSON_free(printed);
    if (printed2) cJSON_free(printed2);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}