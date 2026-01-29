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
//<ID> 1442
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
    cJSON *num0 = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *picked0 = (cJSON *)0;
    cJSON *picked1 = (cJSON *)0;
    cJSON *temp_sum = (cJSON *)0;
    cJSON *final_sum = (cJSON *)0;
    cJSON *note_node = (cJSON *)0;
    char *printed = (char *)0;
    double val0 = 0.0;
    double val1 = 0.0;
    double sum = 0.0;
    int validation_score = 0;
    cJSON_bool added_flag = 0;

    // step 2: Initialize - create root object, array and numeric items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(10.0);
    num1 = cJSON_CreateNumber(32.5);

    // step 3: Configure - populate array, attach to root, and add a small string note
    added_flag = cJSON_AddItemToArray(arr, num0);
    added_flag = cJSON_AddItemToArray(arr, num1);
    added_flag = cJSON_AddItemToObject(root, "values", arr);
    note_node = cJSON_AddStringToObject(root, "note", "initial");

    // step 4: Operate - read numbers from array, compute sum, create a temporary node then replace with final node
    picked0 = cJSON_GetArrayItem(arr, 0);
    picked1 = cJSON_GetArrayItem(arr, 1);
    val0 = cJSON_GetNumberValue(picked0);
    val1 = cJSON_GetNumberValue(picked1);
    sum = val0 + val1;
    temp_sum = cJSON_CreateNumber(sum);
    cJSON_Delete(temp_sum);
    final_sum = cJSON_CreateNumber(sum);
    added_flag = cJSON_AddItemToObject(root, "sum", final_sum);

    // step 5: Validate - print and compute a small validation score
    printed = cJSON_PrintUnformatted(root);
    validation_score = (int)(printed != (char *)0) + (int)(final_sum != (cJSON *)0) + (int)(val0 == 10.0);

    // step 6: Cleanup - free printed buffer and delete the JSON tree
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)picked0;
    (void)picked1;
    (void)note_node;
    (void)added_flag;
    // API sequence test completed successfully
    return 66;
}