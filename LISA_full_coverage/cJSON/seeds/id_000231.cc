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
//<ID> 231
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
    cJSON_AddNumberToObject(root, "pi", 3.1415);
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON *b_true = cJSON_CreateTrue();
    cJSON *s = cJSON_CreateString("example");
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, b_true);
    cJSON_AddItemToArray(arr, s);

    // step 2: Configure
    cJSON_DeleteItemFromArray(arr, 1);
    cJSON *n3 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, n3);

    // step 3: Operate and Validate
    cJSON *maybe_bool = cJSON_GetArrayItem(arr, 1);
    int is_bool = (int)cJSON_IsBool(maybe_bool);
    char *printed = cJSON_Print(root);
    void *tmp = cJSON_malloc(128);
    memset(tmp, 0, 128);
    int summary = cJSON_GetArraySize(arr) + is_bool + (printed ? (int)printed[0] : 0);
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}