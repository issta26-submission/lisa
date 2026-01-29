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
//<ID> 2393
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
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "data", arr);
    cJSON *label = cJSON_CreateString("initial");
    cJSON_AddItemToObject(root, "label", label);
    cJSON *temp = cJSON_CreateString("to_be_detached");
    cJSON_AddItemToObject(root, "temp", temp);

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool has_data = cJSON_HasObjectItem(root, "data");
    cJSON_bool has_temp = cJSON_HasObjectItem(root, "temp");
    buffer[0] = printed ? printed[0] : '\0';
    buffer[1] = (char)(int)(has_data + has_temp);

    // step 3: Operate
    cJSON *detached = cJSON_DetachItemFromObject(root, "temp");
    char *newstr = cJSON_SetValuestring(label, "updated");
    cJSON *data_found = cJSON_GetObjectItem(root, "data");
    cJSON *first = cJSON_GetArrayItem(data_found, 0);
    double first_val = cJSON_GetNumberValue(first);
    buffer[2] = (char)(int)first_val;
    (void)newstr;

    // step 4: Validate and Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}