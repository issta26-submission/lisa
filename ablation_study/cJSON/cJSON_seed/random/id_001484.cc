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
//<ID> 1484
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
    cJSON *elem0 = (cJSON *)0;
    cJSON *elem2 = (cJSON *)0;
    cJSON *note_node = (cJSON *)0;
    char *note_str = (char *)0;
    int arr_size = 0;
    int validation_score = 0;
    cJSON_bool raw0 = 0;
    float numbers[3] = {1.5f, 2.5f, 3.5f};

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateFloatArray(numbers, 3);
    note_node = cJSON_CreateString("array of floats");

    // step 3: Configure
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToObject(root, "note", note_node);

    // step 4: Operate
    arr_size = cJSON_GetArraySize(arr);
    elem0 = cJSON_GetArrayItem(arr, 0);
    elem2 = cJSON_GetArrayItem(arr, 2);
    raw0 = cJSON_IsRaw(elem0);
    note_str = cJSON_GetStringValue(cJSON_GetObjectItem(root, "note"));

    // step 5: Validate
    validation_score = arr_size + (note_str != (char *)0) + (elem2 != (cJSON *)0) + (raw0 ? 0 : 1);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)note_str;
    (void)elem0;
    (void)elem2;
    (void)raw0;
    (void)arr_size;
    // API sequence test completed successfully
    return 66;
}