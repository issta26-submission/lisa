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
//<ID> 874
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *pi = cJSON_CreateNumber(3.141592653589793);
    cJSON *id = cJSON_CreateString("sensor-01");

    // step 2: Configure
    cJSON_AddItemToObject(root, "pi", pi);
    cJSON_AddItemToObject(root, "id", id);
    cJSON *pi_dup = cJSON_Duplicate(pi, 1);
    cJSON_AddItemToObject(root, "pi_dup", pi_dup);

    // step 3: Operate & Validate
    cJSON *got_pi = cJSON_GetObjectItem(root, "pi");
    cJSON_bool is_num = cJSON_IsNumber(got_pi);
    cJSON_bool is_invalid = cJSON_IsInvalid(got_pi);
    cJSON *flag_num = cJSON_CreateBool(is_num);
    cJSON *flag_invalid = cJSON_CreateBool(is_invalid);
    cJSON_AddItemToObject(root, "pi_is_number", flag_num);
    cJSON_AddItemToObject(root, "pi_is_invalid", flag_invalid);
    char *pi_unformatted = cJSON_PrintUnformatted(got_pi);
    cJSON *pi_str = cJSON_CreateString(pi_unformatted ? pi_unformatted : "");
    cJSON_AddItemToObject(root, "pi_str", pi_str);
    cJSON *id_item = cJSON_GetObjectItem(root, "id");
    const char *id_val = cJSON_GetStringValue(id_item);
    cJSON *id_copy = cJSON_CreateString(id_val ? id_val : "");
    cJSON_AddItemToObject(root, "id_copy", id_copy);

    // step 4: Cleanup
    cJSON_free(pi_unformatted);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}