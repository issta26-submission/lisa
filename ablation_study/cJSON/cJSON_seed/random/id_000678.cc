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
//<ID> 678
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *numItem = (cJSON *)0;
    cJSON *strItem = (cJSON *)0;
    cJSON *arr_retrieved = (cJSON *)0;
    cJSON *elem0 = (cJSON *)0;
    cJSON *elem1 = (cJSON *)0;
    cJSON_bool added0 = 0;
    cJSON_bool added1 = 0;
    cJSON_bool added_obj = 0;
    double val_num = 0.0;
    char *val_str = (char *)0;
    int arr_size = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    numItem = cJSON_CreateNumber(123.456);
    strItem = cJSON_CreateString("example");

    // step 3: Configure
    added0 = cJSON_AddItemToArray(arr, numItem);
    added1 = cJSON_AddItemToArray(arr, strItem);
    added_obj = cJSON_AddItemToObject(root, "data", arr);

    // step 4: Operate
    arr_retrieved = cJSON_GetObjectItem(root, "data");
    elem0 = cJSON_GetArrayItem(arr_retrieved, 0);
    elem1 = cJSON_GetArrayItem(arr_retrieved, 1);
    val_num = cJSON_GetNumberValue(elem0);
    val_str = cJSON_GetStringValue(elem1);
    arr_size = cJSON_GetArraySize(arr_retrieved);

    // step 5: Validate
    (void)added0;
    (void)added1;
    (void)added_obj;
    (void)val_num;
    (void)val_str;
    (void)arr_size;
    (void)elem0;
    (void)elem1;
    (void)arr_retrieved;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}