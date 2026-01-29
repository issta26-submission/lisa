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
//<ID> 2395
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(7.0);
    cJSON *num2 = cJSON_CreateNumber(13.0);
    cJSON *str_item = cJSON_CreateString("initial");
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(child, "name", str_item);
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(root, "list", arr);

    // step 2: Configure
    char *printed1 = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool hasChild = cJSON_HasObjectItem(root, "child");
    cJSON_bool hasList = cJSON_HasObjectItem(root, "list");
    buffer[0] = printed1 && printed1[0] ? printed1[0] : '\0';
    buffer[1] = (char)(int)(hasChild + hasList);

    // step 3: Operate
    cJSON *detached = cJSON_DetachItemFromObject(child, "name");
    cJSON_AddItemToArray(arr, detached);
    char *setres = cJSON_SetValuestring(detached, "updated");
    char *printed2 = cJSON_PrintUnformatted(root);
    buffer[2] = printed2 && printed2[0] ? printed2[0] : '\0';
    buffer[3] = setres && setres[0] ? setres[0] : '\0';

    // step 4: Validate and Cleanup
    cJSON_free(printed1);
    cJSON_free(printed2);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}