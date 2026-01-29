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
//<ID> 255
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *ints = (cJSON *)0;
    cJSON *pi = (cJSON *)0;
    cJSON *unit_str = (cJSON *)0;
    cJSON *meta = (cJSON *)0;
    cJSON *item0 = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    cJSON *sum_item = (cJSON *)0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    cJSON_bool add_ok2 = (cJSON_bool)0;
    double val0 = 0.0;
    double val1 = 0.0;
    double sum = 0.0;
    char *printed = (char *)0;
    int numbers[4];
    numbers[0] = 10;
    numbers[1] = 20;
    numbers[2] = 30;
    numbers[3] = 40;

    // step 2: Initialize
    root = cJSON_CreateObject();

    // step 3: Configure - create an int array, add number and string entries, attach a meta object
    ints = cJSON_CreateIntArray(numbers, 4);
    add_ok1 = cJSON_AddItemToObject(root, "ints", ints);
    pi = cJSON_CreateNumber(3.14159);
    add_ok2 = cJSON_AddItemToObject(root, "pi", pi);
    unit_str = cJSON_AddStringToObject(root, "unit", "metric");
    meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "note", "sample");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 4: Operate - read values from the array and number, compute a sum and add it to the root
    item0 = cJSON_GetArrayItem(ints, 0);
    val0 = cJSON_GetNumberValue(item0);
    item1 = cJSON_GetArrayItem(ints, 1);
    val1 = cJSON_GetNumberValue(item1);
    sum = val0 + val1 + cJSON_GetNumberValue(pi);
    sum_item = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum", sum_item);

    // step 5: Validate - serialize and free the printed string
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}