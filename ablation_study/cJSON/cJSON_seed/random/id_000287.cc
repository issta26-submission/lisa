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
//<ID> 287
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
    cJSON *false_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    char json_buf[] = "  { \"trim\" : \"  value \" , \"flag\": false }  ";
    char *printed = (char *)0;
    cJSON_bool is_false_flag = (cJSON_bool)0;
    double retrieved = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "array", arr);

    // step 3: Configure
    cJSON_AddItemReferenceToArray(arr, false_item);
    cJSON_Minify(json_buf);

    // step 4: Operate
    is_false_flag = cJSON_IsFalse(false_item);
    num_item = cJSON_CreateNumber((double)is_false_flag);
    cJSON_AddItemToObject(root, "is_false", num_item);
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 5: Validate
    retrieved = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "is_false"));

    // step 6: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}