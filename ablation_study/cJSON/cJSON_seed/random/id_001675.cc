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
//<ID> 1675
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
    cJSON *detached = (cJSON *)0;
    cJSON *getitem = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool is_false = 0;
    double first_val = 0.0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(3.14);
    n1 = cJSON_CreateNumber(0.0);

    // step 3: Configure
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToObject(root, "data", arr);

    // step 4: Operate
    detached = cJSON_DetachItemFromObjectCaseSensitive(root, "data");
    getitem = cJSON_GetArrayItem(detached, 1);
    first_val = cJSON_GetNumberValue(cJSON_GetArrayItem(detached, 0));
    is_false = cJSON_IsFalse(getitem);
    printed = cJSON_PrintUnformatted(detached);

    // step 5: Validate
    validation = (int)(first_val == 3.14) + (int)(printed != (char *)0) + (int)is_false;
    (void)validation;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)n0;
    (void)n1;
    (void)detached;
    (void)getitem;
    (void)is_false;
    (void)first_val;
    (void)printed;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}