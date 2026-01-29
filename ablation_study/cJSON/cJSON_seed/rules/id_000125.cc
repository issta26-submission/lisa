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
//<ID> 125
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *root_copy = cJSON_CreateObject();
    cJSON *raw = cJSON_CreateRaw("{\"info\":\"raw\"}");
    cJSON *num_ptr = NULL;
    cJSON *num_ptr_copy = NULL;
    char *printed = NULL;
    double extracted_value = 0.0;
    cJSON_bool are_equal = 0;
    cJSON_bool is_raw_flag = 0;

    // step 2: Setup / Configure
    num_ptr = cJSON_AddNumberToObject(root, "value", 3.1415);
    num_ptr_copy = cJSON_AddNumberToObject(root_copy, "value", 3.1415);
    cJSON_AddItemToObject(root, "meta", raw);

    // step 3: Operate / Validate
    extracted_value = cJSON_GetNumberValue(num_ptr);
    are_equal = cJSON_Compare(root, root_copy, 1);
    is_raw_flag = cJSON_IsRaw(raw);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root_copy);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}