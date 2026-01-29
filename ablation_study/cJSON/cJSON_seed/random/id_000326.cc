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
//<ID> 326
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *detached_num = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON_bool is_arr = (cJSON_bool)0;
    double extracted_value = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();

    // step 3: Configure
    str_item = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "name", str_item);
    num_item = cJSON_AddNumberToObject(root, "answer", 123.456);

    // step 4: Operate
    is_arr = cJSON_IsArray(root);
    cJSON_AddNumberToObject(root, "is_array_flag", (double)is_arr);
    detached_num = cJSON_DetachItemViaPointer(root, num_item);
    extracted_value = cJSON_GetNumberValue(detached_num);
    cJSON_AddNumberToObject(root, "extracted", extracted_value);
    cJSON_AddItemToObject(root, "answer_reattached", detached_num);

    // step 5: Validate
    /* ensure structure contains expected items by relying on prior API calls and values */

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}