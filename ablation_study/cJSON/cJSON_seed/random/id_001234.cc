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
//<ID> 1234
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
    cJSON *true_item = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool is_raw = 0;
    int validation_score = 0;

    // step 2: Initialize - create an object, an array and a true boolean
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    true_item = cJSON_CreateTrue();

    // step 3: Configure - add the boolean to the array and attach the array to the object
    cJSON_AddItemToArray(arr, true_item);
    cJSON_AddItemToObject(root, "payload", arr);

    // step 4: Operate - check raw flag and produce a buffered print of the root object
    is_raw = cJSON_IsRaw(true_item);
    printed = cJSON_PrintBuffered(root, 256, 1);

    // step 5: Validate - compute a simple validation score from meaningful data flow
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(true_item != (cJSON *)0) + (int)(printed != (char *)0) + (int)(!is_raw);
    (void)validation_score;

    // step 6: Cleanup - free printed buffer and delete the root (which frees children)
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}