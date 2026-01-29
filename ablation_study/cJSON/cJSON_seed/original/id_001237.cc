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
//<ID> 1237
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
    cJSON_AddItemToObject(root, "list", arr);

    // step 2: Configure
    cJSON *num1 = cJSON_CreateNumber(3.14159);
    cJSON_AddItemReferenceToArray(arr, num1);
    cJSON_AddItemReferenceToObject(root, "pi_ref", num1);
    cJSON *num2 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, num2);

    // step 3: Operate
    int size_before = cJSON_GetArraySize(arr);
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = printed ? strlen(printed) : 0;
    char *buf = (char *)cJSON_malloc(printed_len + 32);
    memset(buf, 0, printed_len + 32);
    if (printed) memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_DeleteItemFromArray(arr, 1);
    int size_after = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "count_after_delete", (double)size_after);

    // step 4: Validate & Cleanup
    (void)size_before;
    (void)size_after;
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}