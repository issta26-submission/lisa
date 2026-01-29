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
//<ID> 2394
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
    cJSON *s1 = cJSON_CreateString("one");
    cJSON *s2 = cJSON_CreateString("two");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToObject(root, "array", arr);

    // step 2: Configure
    cJSON_AddItemToArray(arr, cJSON_CreateString("three"));
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed && printed[0] ? printed[0] : '\0';

    // step 3: Operate
    cJSON_bool has_arr_before = cJSON_HasObjectItem(root, "array");
    cJSON *detached = cJSON_DetachItemFromObject(root, "array");
    cJSON_bool has_arr_after = cJSON_HasObjectItem(root, "array");
    char *setres = cJSON_SetValuestring(s1, "ONE");
    char *printed_detached = cJSON_PrintUnformatted(detached);
    buffer[1] = printed_detached && printed_detached[0] ? printed_detached[0] : '\0';
    buffer[2] = (char)(int)(has_arr_before + has_arr_after);
    (void)setres;

    // step 4: Validate and Cleanup
    cJSON_free(printed);
    cJSON_free(printed_detached);
    cJSON_free(buffer);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}