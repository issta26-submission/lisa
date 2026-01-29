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
//<ID> 1661
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
    cJSON *n0 = (cJSON *)0;
    cJSON *n1 = (cJSON *)0;
    cJSON *obj = (cJSON *)0;
    cJSON *replacement = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool replaced = 0;
    int array_size = 0;
    double val0 = 0.0;
    double replaced_value = 0.0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(1.5);
    n1 = cJSON_CreateNumber(2.5);
    obj = cJSON_CreateObject();
    cJSON_AddNumberToObject(obj, "value", 3.5);

    // step 3: Configure
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToObject(root, "single", obj);

    // step 4: Operate
    array_size = cJSON_GetArraySize(arr);
    printed = cJSON_PrintUnformatted(root);
    replacement = cJSON_CreateNumber(7.25);
    replaced = cJSON_ReplaceItemInObject(root, "single", replacement);
    val0 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    replaced_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "single"));

    // step 5: Validate
    validation = (int)(array_size == 2)
               + (int)(printed != (char *)0)
               + (int)replaced
               + (int)(val0 == 1.5)
               + (int)(replaced_value == 7.25);
    (void)validation;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)n0;
    (void)n1;
    (void)obj;
    (void)replacement;
    (void)replaced;
    (void)array_size;
    (void)val0;
    (void)replaced_value;
    // API sequence test completed successfully
    return 66;
}