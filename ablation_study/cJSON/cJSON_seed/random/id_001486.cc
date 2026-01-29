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
//<ID> 1486
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
    cJSON *elem1 = (cJSON *)0;
    cJSON *str_node = (cJSON *)0;
    cJSON *raw_node = (cJSON *)0;
    char *printed = (char *)0;
    char *note_value = (char *)0;
    float numbers[3];
    int arr_size = 0;
    int validation_score = 0;
    cJSON_bool raw_flag = 0;

    // step 2: Initialize - prepare float data and create JSON structures
    numbers[0] = 1.1f;
    numbers[1] = 2.2f;
    numbers[2] = 3.3f;
    root = cJSON_CreateObject();
    arr = cJSON_CreateFloatArray(numbers, 3);
    cJSON_AddItemToObject(root, "floats", arr);
    str_node = cJSON_AddStringToObject(root, "note", "three floats");
    raw_node = cJSON_AddRawToObject(root, "meta", "{\"type\":\"float_array\"}");

    // step 3: Operate - inspect array and items
    arr_size = cJSON_GetArraySize(arr);
    elem1 = cJSON_GetArrayItem(arr, 1);
    raw_flag = cJSON_IsRaw(raw_node);
    note_value = cJSON_GetStringValue(str_node);

    // step 4: Validate - produce a printed representation and compute a small score
    printed = cJSON_PrintUnformatted(root);
    validation_score = arr_size + (int)(elem1 != (cJSON *)0) + (int)raw_flag + (int)(note_value != (char *)0) + (int)(printed != (char *)0);

    // step 5: Cleanup - free printed buffer and delete the JSON tree
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)note_value;
    (void)elem1;
    (void)arr_size;
    (void)raw_flag;
    (void)numbers;
    // API sequence test completed successfully
    return 66;
}