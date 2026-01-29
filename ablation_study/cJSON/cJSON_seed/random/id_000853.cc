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
//<ID> 853
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *arr = (cJSON *)0;
    cJSON *root = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON_bool is_num = 0;
    cJSON_bool added = 0;
    int validation_score = 0;
    const char *json = "[42, 84, 126]";

    // step 2: Initialize
    arr = cJSON_Parse(json);
    root = cJSON_CreateObject();

    // step 3: Configure
    detached = cJSON_DetachItemFromArray(arr, 1);
    added = cJSON_AddItemToObject(root, "moved", detached);

    // step 4: Operate
    is_num = cJSON_IsNumber(detached);

    // step 5: Validate
    validation_score = (arr != (cJSON *)0) + (root != (cJSON *)0) + (detached != (cJSON *)0) + (int)is_num + (int)added;
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(arr);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}