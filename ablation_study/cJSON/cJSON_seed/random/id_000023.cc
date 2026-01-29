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
//<ID> 23
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char json_buf[] = "  [ \"alpha\" , \"beta\" ]  ";
    cJSON *root_obj = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON *detached_arr = (cJSON *)0;
    cJSON *parsed = (cJSON *)0;
    cJSON_bool invalid_flag = 0;
    int arr_size = 0;

    // step 2: Setup
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemReferenceToObject(root_obj, "shared_array", arr);

    // step 3: Operate
    cJSON_Minify(json_buf);
    parsed = cJSON_Parse(json_buf);
    invalid_flag = cJSON_IsInvalid(parsed);
    arr_size = cJSON_GetArraySize(arr);

    // step 4: Validate
    detached_arr = cJSON_DetachItemFromObject(root_obj, "shared_array");

    // step 5: Cleanup
    cJSON_Delete(detached_arr);
    cJSON_Delete(parsed);
    cJSON_Delete(root_obj);

    // API sequence test completed successfully
    return 66;
}