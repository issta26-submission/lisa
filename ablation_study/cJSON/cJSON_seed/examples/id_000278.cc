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
//<ID> 278
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
    cJSON *btrue = cJSON_CreateBool(1);
    cJSON *bfalse = cJSON_CreateFalse();
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON *label = cJSON_CreateString("example");

    // step 2: Configure
    cJSON_AddItemToArray(arr, num);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(3.14));
    cJSON_AddItemToArray(arr, bfalse);
    cJSON_AddItemToObjectCS(root, "data", arr);
    cJSON_AddItemToObjectCS(root, "flag", btrue);
    cJSON_AddItemToObjectCS(root, "label", label);

    // step 3: Operate & Validate
    int sz = cJSON_GetArraySize(arr);
    cJSON *size_num = cJSON_CreateNumber((double)sz);
    cJSON_AddItemToObjectCS(root, "size", size_num);
    cJSON *true_check = cJSON_CreateBool(cJSON_IsTrue(btrue));
    cJSON_AddItemToObjectCS(root, "flag_is_true", true_check);
    void *buffer = cJSON_malloc((size_t)256);
    memset(buffer, 0, 256);
    char *bufc = (char *)buffer;
    cJSON_PrintPreallocated(root, bufc, 256, 1);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(buffer);
    return 66; // API sequence test completed successfully
}