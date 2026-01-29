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
//<ID> 2398
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
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "array", arr);
    cJSON *temp = cJSON_CreateObject();
    cJSON *val = cJSON_CreateString("initial");
    cJSON_AddItemToObject(temp, "value", val);
    cJSON_AddItemToObject(root, "temp", temp);

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed && printed[0] ? printed[0] : '\0';
    cJSON_bool has_temp = cJSON_HasObjectItem(root, "temp");
    cJSON_bool has_array = cJSON_HasObjectItem(root, "array");
    buffer[3] = (char)(int)(has_temp + has_array);

    // step 3: Operate
    cJSON *val_found = cJSON_GetObjectItem(temp, "value");
    char *setres = cJSON_SetValuestring(val_found, "updated");
    cJSON *detached = cJSON_DetachItemFromObject(root, "array");
    cJSON_AddItemToObject(root, "array2", detached);
    cJSON_AddItemToArray(detached, cJSON_CreateNumber(3.0));
    int arrsize = cJSON_GetArraySize(detached);
    buffer[1] = (char)arrsize;
    buffer[2] = setres && setres[0] ? setres[0] : '\0';

    // step 4: Validate and Cleanup
    char *final_print = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_free(final_print);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}