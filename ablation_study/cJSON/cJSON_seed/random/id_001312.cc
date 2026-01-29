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
//<ID> 1312
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
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *num3 = (cJSON *)0;
    cJSON *avg_item = (cJSON *)0;
    cJSON *retrieved_avg = (cJSON *)0;
    char *printed = (char *)0;
    double v1 = 0.0;
    double v2 = 0.0;
    double v3 = 0.0;
    double avg = 0.0;
    double avg_read = 0.0;
    cJSON_bool add_res1 = (cJSON_bool)0;
    cJSON_bool add_res2 = (cJSON_bool)0;
    cJSON_bool add_res3 = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Initialize - create root object, an array and three numeric items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(10.5);
    num2 = cJSON_CreateNumber(20.25);
    num3 = cJSON_CreateNumber(5.25);

    // step 3: Configure - attach the array to root and populate the array with the numeric items
    cJSON_AddItemToObject(root, "values", arr);
    add_res1 = cJSON_AddItemToArray(arr, num1);
    add_res2 = cJSON_AddItemToArray(arr, num2);
    add_res3 = cJSON_AddItemToArray(arr, num3);

    // step 4: Operate - read numeric values, compute average, add it to root, retrieve it via object lookup, serialize unformatted
    v1 = cJSON_GetNumberValue(num1);
    v2 = cJSON_GetNumberValue(num2);
    v3 = cJSON_GetNumberValue(num3);
    avg = (v1 + v2 + v3) / 3.0;
    avg_item = cJSON_AddNumberToObject(root, "average", avg);
    retrieved_avg = cJSON_GetObjectItem(root, "average");
    avg_read = cJSON_GetNumberValue(retrieved_avg);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate - produce a simple validation score from results
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(add_res1 != 0) + (int)(add_res2 != 0) + (int)(add_res3 != 0) + (int)(avg_item != (cJSON *)0) + (int)(retrieved_avg != (cJSON *)0) + (int)(printed != (char *)0) + (int)(avg_read == avg);
    (void)validation_score;

    // step 6: Cleanup - free printed buffer and delete the root (which frees all children)
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}