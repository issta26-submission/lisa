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
//<ID> 1443
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
    cJSON *sum = (cJSON *)0;
    cJSON *picked = (cJSON *)0;
    char *printed = (char *)0;
    double val0 = 0.0;
    double val1 = 0.0;
    int validation_score = 0;

    // step 2: Initialize - create root object, an array and two numeric nodes
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(3.5);
    num1 = cJSON_CreateNumber(6.5);

    // step 3: Configure - populate the array and add descriptive string to root
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddStringToObject(root, "description", "sample numbers");
    cJSON_AddItemToObject(root, "values", arr);

    // step 4: Operate - read numbers from the array, create a sum node and attach it
    picked = cJSON_GetArrayItem(arr, 0);
    val0 = cJSON_GetNumberValue(picked);
    val1 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1));
    sum = cJSON_CreateNumber(val0 + val1);
    cJSON_AddItemToObject(root, "sum", sum);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate - form a small validation score from observable results
    validation_score = (int)(printed != (char *)0) + (int)(cJSON_GetObjectItem(root, "sum") != (cJSON *)0) + (int)(cJSON_GetObjectItem(root, "description") != (cJSON *)0);

    // step 6: Cleanup - free printed buffer and delete the JSON tree
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    (void)validation_score;
    (void)val0;
    (void)val1;
    // API sequence test completed successfully
    return 66;
}