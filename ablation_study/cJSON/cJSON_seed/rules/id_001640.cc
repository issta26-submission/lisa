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
//<ID> 1640
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *ref = NULL;
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *found_ref = NULL;
    char *printed = NULL;
    int arr_size = 0;

    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(3.14);
    num2 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "numbers", arr);
    ref = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToObject(root, "numbers_ref", ref);

    found_ref = cJSON_GetObjectItem(root, "numbers_ref");
    arr_size = cJSON_GetArraySize(found_ref);
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    cJSON_Delete(root);

    return 66;
}