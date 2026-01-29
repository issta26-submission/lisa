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
//<ID> 261
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *num = cJSON_CreateNumber(7.5);
    cJSON_AddItemToArray(arr, num);
    cJSON *num_dup = cJSON_Duplicate(num, 1);
    cJSON_AddItemToArray(arr, num_dup);
    int size = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "array_size", (double)size);
    cJSON *flag = cJSON_AddFalseToObject(root, "all_good");
    char *printed = cJSON_PrintUnformatted(root);
    void *tmp = cJSON_malloc(128);
    (void)flag;
    (void)printed;
    (void)tmp;
    cJSON_free(printed);
    cJSON_free(tmp);
    cJSON_Delete(root);
    return 66;
}