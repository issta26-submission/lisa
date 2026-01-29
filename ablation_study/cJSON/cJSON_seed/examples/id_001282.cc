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
//<ID> 1282
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *orig_arr = cJSON_CreateArray();
    cJSON *str1 = cJSON_CreateString("hello");
    cJSON *num1 = cJSON_CreateNumber(42);
    cJSON *num2 = cJSON_CreateNumber(7);

    // step 2: Configure
    cJSON_AddItemToArray(orig_arr, num1);
    cJSON_AddItemToArray(orig_arr, num2);
    cJSON_AddItemToObject(root, "original", orig_arr);
    cJSON_AddItemToObject(root, "greeting", str1);
    // create a safe duplicate for the alias to avoid double-free issues when deleting root
    cJSON *arr_ref = cJSON_Duplicate(orig_arr, 1);
    cJSON_AddItemToObject(root, "alias", arr_ref);

    // step 3: Operate & Validate
    char *printed = cJSON_Print(root);
    const char *greeting = cJSON_GetStringValue(str1);
    void *buf = cJSON_malloc(16);
    memset(buf, 0, 16);
    ((char*)buf)[0] = greeting ? greeting[0] : '\0';
    cJSON_free(printed);
    cJSON_free(buf);

    // step 4: Cleanup
    cJSON_Delete(root);

    return 66;
}