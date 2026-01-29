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
//<ID> 956
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *array_key = "my_array";
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *null_item = (cJSON *)0;
    cJSON *fetched = (cJSON *)0;
    char *json = (char *)0;
    cJSON_bool add_ok = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    null_item = cJSON_CreateNull();

    // step 3: Configure
    add_ok = cJSON_AddItemToArray(arr, null_item);
    cJSON_AddItemToObject(root, array_key, arr);

    // step 4: Operate
    fetched = cJSON_GetObjectItem(root, array_key);
    json = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation_score = (int)(root != (cJSON *)0) + (int)add_ok + (int)(fetched != (cJSON *)0) + (int)(json != (char *)0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_free((void *)json);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}