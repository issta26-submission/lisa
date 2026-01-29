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
//<ID> 1120
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
    cJSON *num_item = (cJSON *)0;
    cJSON *fetched_num = (cJSON *)0;
    char *printed = (char *)0;
    double retrieved_num = 0.0;
    cJSON_bool is_num = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create root object and an array, attach array using case-sensitive API, add a number to the object
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_item = cJSON_AddNumberToObject(root, "count", 123.0);
    cJSON_AddItemToObjectCS(root, "values", arr);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(2.0));

    // step 3: Operate - fetch the numeric item, inspect and serialize the object
    fetched_num = cJSON_GetObjectItem(root, "count");
    is_num = cJSON_IsNumber(fetched_num);
    retrieved_num = cJSON_GetNumberValue(fetched_num);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Validate - perform simple checks and free serialized buffer
    validation_score = (int)(root != (cJSON *)0) + (int)is_num + (int)(retrieved_num == 123.0);
    cJSON_free((void *)printed);

    // step 5: Cleanup - delete root to free all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}