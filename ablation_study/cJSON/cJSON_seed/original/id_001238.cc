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
//<ID> 1238
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
    cJSON_AddItemToObject(root, "numbers", arr);

    // step 2: Configure
    cJSON *num1 = cJSON_CreateNumber(3.1415);
    cJSON *num2 = cJSON_CreateNumber(2.7182);
    cJSON_bool ok_ref1 = cJSON_AddItemReferenceToArray(arr, num1);
    cJSON_bool ok_ref2 = cJSON_AddItemReferenceToObject(root, "e", num2);

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = printed ? strlen(printed) : 0;
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_DeleteItemFromArray(arr, 0);
    double e_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "e"));
    int arr_size_after = cJSON_GetArraySize(arr);
    (void)ok_ref1;
    (void)ok_ref2;
    (void)e_value;
    (void)arr_size_after;

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}