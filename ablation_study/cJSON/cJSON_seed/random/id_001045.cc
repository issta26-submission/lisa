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
//<ID> 1045
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
    cJSON *label = (cJSON *)0;
    cJSON *fetched = (cJSON *)0;
    cJSON_bool ok0 = (cJSON_bool)0;
    cJSON_bool ok1 = (cJSON_bool)0;
    int array_size = 0;
    double val0 = 0.0;
    double val1 = 0.0;
    int validation_score = 0;

    // step 2: Setup - create object, array, numbers and a string
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(42.0);
    n1 = cJSON_CreateNumber(7.5);
    label = cJSON_CreateString("example-label");

    // step 3: Configure - assemble JSON structure and add the case-sensitive string entry
    ok0 = cJSON_AddItemToArray(arr, n0);
    ok1 = cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToObjectCS(root, "label", label);

    // step 4: Operate - query array size and extract numeric values
    array_size = cJSON_GetArraySize(arr);
    fetched = cJSON_GetArrayItem(arr, 0);
    val0 = cJSON_GetNumberValue(fetched);
    fetched = cJSON_GetArrayItem(arr, 1);
    val1 = cJSON_GetNumberValue(fetched);

    // step 5: Validate - compute a small validation score using results
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(n0 != (cJSON *)0) + (int)(n1 != (cJSON *)0) + (int)(label != (cJSON *)0) + (int)ok0 + (int)ok1 + (int)(array_size == 2) + (int)(val0 == 42.0) + (int)(val1 == 7.5);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}