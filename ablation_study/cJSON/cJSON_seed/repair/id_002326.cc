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
//<ID> 2326
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
    const char *ref_text = "hello-ref";
    cJSON *ref = cJSON_CreateStringReference(ref_text);
    cJSON_AddItemToArray(arr, ref);
    cJSON *num = cJSON_CreateNumber(42.5);
    cJSON_AddItemToArray(arr, num);
    cJSON *obj = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", obj);
    cJSON_AddStringToObject(obj, "owner", "tester");

    // step 2: Configure
    cJSON_AddItemToObject(root, "answer", cJSON_CreateNumber(7.0));
    cJSON *arr_copy = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToObject(root, "arr_copy", arr_copy);

    // step 3: Operate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    const char *first_str = cJSON_GetStringValue(first);
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    double second_val = cJSON_GetNumberValue(second);
    char *json = cJSON_PrintUnformatted(root);

    // step 4: Validate and Cleanup
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (first_str && first_str[0]) ? first_str[0] : '\0';
    buffer[2] = (char)('0' + ((int)second_val % 10));
    cJSON_free(json);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}