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
//<ID> 1275
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json_text = "{\"numbers\":[10,20,30]}";
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *meta = (cJSON *)0;
    int arr_size = 0;
    int validation_score = 0;
    cJSON_bool processed_flag = 1;

    // step 2: Initialize - parse input JSON into a root object
    root = cJSON_Parse(json_text);

    // step 3: Configure - obtain the array, create a meta object and add a boolean to it, attach meta to root
    arr = cJSON_GetObjectItem(root, "numbers");
    meta = cJSON_CreateObject();
    cJSON_AddBoolToObject(meta, "processed", processed_flag);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 4: Operate - measure the array size
    arr_size = cJSON_GetArraySize(arr);

    // step 5: Validate - compute a small validation score to ensure data flowed through APIs
    validation_score = arr_size + (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(meta != (cJSON *)0) + (int)processed_flag;
    (void)validation_score;

    // step 6: Cleanup - delete the root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}