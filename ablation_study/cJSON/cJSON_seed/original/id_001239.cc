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
//<ID> 1239
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

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(42.0);
    cJSON_AddItemReferenceToArray(arr, n1);
    cJSON *n2 = cJSON_CreateNumber(100.5);
    cJSON_AddItemReferenceToObject(root, "hundred", n2);
    cJSON *direct = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(arr, direct);
    cJSON_DeleteItemFromArray(arr, 0);

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 16);
    memset(buf, 0, printed_len + 16);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON *first_item = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON *doubled = cJSON_CreateNumber(first_val * 2.0);
    cJSON_AddItemToArray(arr, doubled);

    // step 4: Validate & Cleanup
    cJSON *hundred_item = cJSON_GetObjectItem(root, "hundred");
    double hundred_val = cJSON_GetNumberValue(hundred_item);
    cJSON_AddNumberToObject(root, "sum", first_val + hundred_val);
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}