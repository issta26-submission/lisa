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
//<ID> 610
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
    cJSON *sref = (cJSON *)0;
    cJSON *sitem = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    char *first_str = (char *)0;
    cJSON_bool is_false = 0;
    double count_val = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    sref = cJSON_CreateStringReference("alpha");
    sitem = cJSON_CreateString("beta");
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    cJSON_AddItemToArray(arr, sref);
    cJSON_AddItemToArray(arr, sitem);
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddItemToObject(root, "flag", false_item);
    num_item = cJSON_AddNumberToObject(root, "count", 2.0);
    cJSON_AddItemReferenceToObject(root, "first_ref", sref);

    // step 4: Operate
    is_false = cJSON_IsFalse(false_item);
    count_val = cJSON_GetNumberValue(num_item);
    first_str = cJSON_GetStringValue(cJSON_GetObjectItem(root, "first_ref"));

    // step 5: Validate
    (void)is_false;
    (void)count_val;
    (void)first_str;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}